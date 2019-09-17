#include <pe.h>
#include <fonctions.h>
#include <cpux86.h>

void exec(long ptr)
{
    static void (*pf)(void) = 0;
    static int comp = 0;
	static struct IMAGE_DOS_HEADER *dos_header=0;
    static struct Header *header = 0;
    static struct OptionalHeader *op_header = 0;
    static struct SectionHeadersEntry *sec_head_entry = 0;

    dos_header = (struct IMAGE_DOS_HEADER *)(ptr);
    header = (struct Header *)(dos_header->e_lfanew + ptr);
    op_header = (struct OptionalHeader *)((long)header + 24);
    sec_head_entry = (struct SectionHeadersEntry *)((long)op_header + header->SizeOfOptionalHeader);
/*
hexl(header->NumberOfSection);
hexl(header->SizeOfOptionalHeader);
hexl(op_header->AddressOfEntryPoint);
hexl(op_header->ImageBase);
hexl(op_header->SizeOfHeaders);
//print(sec_head_entry->name);putcar(10);
hexl(sec_head_entry->VirtualAddress);
hexl(sec_head_entry->SizeOfRawData);
hexl(sec_head_entry->PointerToRawData);
*/
    for(comp = 0; comp <header->NumberOfSection; comp++)
    {
/*
Copie des sections à leur place en memoire
Filtrage de la section .comment ajouter par nasm.
Filtering .comment section added by nasm.
*/
		if (CpuCmpStringC(".comment",sec_head_entry[comp].name,8)!=0)
		{
				 CpuCopyStringW((void*)(ptr + sec_head_entry[comp].PointerToRawData),
				 (void*)(op_header->ImageBase + sec_head_entry[comp].VirtualAddress),
				 sec_head_entry[comp].SizeOfRawData);
		}
//hexl((long)(sec_head_entry[comp].SizeOfRawData/2));
//hexl((long)sec_head_entry);
//hex(comp);
    }
    pf = (void (*)(void))(op_header->AddressOfEntryPoint + op_header->ImageBase);
//hexl((int) pf);
    (*pf)(); //Jump to 401000
}
