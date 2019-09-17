/*

Le chargement de DLL en 64 bits ne fonctionne pas
à cause de NX bit qui empeche d'executer du code  dans un segment DATA.

Donc impossible d'appelé une fonction de la DLL de test en 64 bits.

Executez en 32 bits.

*/

//#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

//#/#include <cpux86.h>
//#include <textmode.h>
#include<pe.h>
#include<clr.h>

//extern int ValueX4(int);
int load(void*);
void reloc(void*);
void LinkToLib(void *, char *);
void hexl(long);

static struct IMAGE_DOS_HEADER *dos_header=0;
static struct Header *header = 0;
static struct OptionalHeader64 *op_header = 0;
static struct SectionHeadersEntry* sec_head_entry = 0;

void *buff2;
long size_img=0;

int main(void)
{
	void *buff;
	ulong f_size;
	int lib;
	static int (*pf)(int) = 0;
	static char* (*pf2)(void) = 0;

	lib=open("libtest.dll", O_RDONLY);
	if(lib==0){printf("libtest.dll %s\n",strerror(errno));return 0;}

	f_size=lseek(lib,0,SEEK_END);

	printf("Size is: %ld\n",f_size);
	printf("Size of long: %ld\n",(long)sizeof(long));

	lseek(lib,0,0);
	buff=malloc((f_size%4096+1)*4096);
	read(lib,buff,f_size);

	if(load(buff)){return 1;}
	reloc(buff);
	LinkToLib(buff2,"ValueX3");
	LinkToLib(buff2,"ValueString");
//	pf = (int (*)(int))((long)ptr + 0x1110);//32b
//	pf = (int (*)(int))((long)ptr + 0x1090);//64b
//	printf("Resultat %d\n",(*pf)(4));

//	pf2 = (char* (*)(void))((long)ptr + 0x112c);//32b
//	pf2 = (char* (*)(void))((long)ptr + 0x10b4);//64b
//	printf("Resultat %s\n",(*pf2)());

	free(buff);
	munmap(buff2,size_img);
	close(lib);
	return 0;
}

int load(void *buff)
{
	long comp=0;

    dos_header = (struct IMAGE_DOS_HEADER *)(buff);
    header = (struct Header *)(dos_header->e_lfanew + buff);
    op_header = (struct OptionalHeader64 *)((long)header + sizeof(struct Header));
    sec_head_entry = (struct SectionHeadersEntry *)((long)op_header + header->SizeOfOptionalHeader);

	size_img=op_header->SizeOfImage;

	//reservation memoire
	buff2=mmap(NULL,op_header->SizeOfImage,PROT_EXEC|PROT_READ|PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);
	if(buff2==MAP_FAILED)
	{
		printf("MMAP Error: %s\n",strerror(errno));
		return 1;
	}

//printf("Buffer2 add: %lx\n",buff2);
//printf("Buffer2 img size: %lx\n",op_header->SizeOfImage);
	for(comp = 0; comp <header->NumberOfSection; comp++)
    {
//		printf("Section %ld name is %s\n",comp,sec_head_entry[comp].name);
		memcpy((char*)(buff2 + sec_head_entry[comp].VirtualAddress),
				(char*)(buff + sec_head_entry[comp].PointerToRawData),
				sec_head_entry[comp].SizeOfRawData);
	}
	return 0;
}

void LinkToLib(void *ptr, char *fontion)
{
	
}

void reloc(void *buff)
{
    static ulong comp=0,comp2=0,b_r_b_size=0;
	static long reloc=0, *ptr=0;

    static struct BaseRelocationBlock* b_r_b = 0;
    static struct BaseRelocationEntry* b_r_e = 0;

    dos_header = (struct IMAGE_DOS_HEADER *)(buff);
    header = (struct Header *)(dos_header->e_lfanew + buff);
    op_header = (struct OptionalHeader64 *)((long)header + sizeof(struct Header));
    sec_head_entry = (struct SectionHeadersEntry *)((long)op_header + header->SizeOfOptionalHeader);

	reloc=(long)buff2-op_header->ImageBase;
//	printf("BRB reloc %lx\n",(long)reloc);
//	printf("Buffer2 %lx\n",(long)buff2);
//	printf("Image Base %lx\n",(long)op_header->ImageBase);
	for(comp = 0; comp <header->NumberOfSection; comp++)
    {
//		printf("Section %ld name is %s\n",comp,sec_head_entry[comp].name);

		if (strncmp(".reloc",(char*)sec_head_entry[comp].name,8)==0)
		{
//			printf("Relocation table\n");
			b_r_b=buff + sec_head_entry[comp].PointerToRawData+b_r_b_size;
			while(b_r_b->page_rva!=0)
			{
//				printf("BRB %lx\n",(void*)b_r_b-buff);
//				printf("BRB page_rva %x\n",b_r_b->page_rva);
//				printf("BRB block_size %x\n",b_r_b->block_size);
				b_r_e=(void*)b_r_b+sizeof(struct BaseRelocationBlock);
				for(comp2=0;(b_r_e[comp2].offset!=0) | (b_r_e[comp2].type!=0);comp2++)
				{
//					printf("BRE\n");
//					printf("Type %x\n",b_r_e[comp2].type);
					switch(b_r_e[comp2].type & 0xf)
					{
						case IMAGE_REL_BASED_HIGHLOW: //LowHigh 32b relocation
						case IMAGE_REL_BASED_DIR64: //LowHigh 64b relocation
							ptr=(buff2+(b_r_e[comp2].offset+b_r_b->page_rva));
//							printf("Relocation %lx\n",(long)ptr);
//							printf("Relocation %lx\n",(long)*ptr);
							*ptr=*ptr+reloc;
//							printf("Relocation %lx\n",(long)*ptr);
						break;
					}
					b_r_e=(void*)b_r_b+sizeof(struct BaseRelocationBlock);
				}
				b_r_b=(void*)b_r_b+b_r_b->block_size;
//				printf("BRB new loc %lx\n",(long)b_r_b-(long)buff);
				//getchar();
			}
		}
    }
}

void hexl(long var)
{
	printf("Hex: 0x%lx\n",var);
}