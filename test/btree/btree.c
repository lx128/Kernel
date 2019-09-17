#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include"Btree.h"

#define MAX_FEUILLE	4

#define NO_ERR 0
#define ERR_NOTFOUND 1

void MemBlank(char*, ulong);

ulong BTreeDataInsert(ulong, char*);
void BTreeDataUpdate(ulong,ulong, char*);
ulong BTreeDataDelete(ulong);
struct Data* BTreeDataFind(ulong, ulong);

ulong BTreeBuild(ulong);
ulong BTreePlusBuild(ulong);
struct Data* BTreeSearch(ulong);
struct Data* BTreePlusSearch(ulong);

void BTreeDataWstringInsert(ulong, char*,ulong, char*);

char error;
void* buffer;
struct Header *header;

int main()
{
	int disk,comp;
	struct Data* dt;

	disk=open("arbre.bin",O_CREAT|O_RDWR,0666);
	buffer = malloc(4096);
	read(disk,buffer,4096);
	header=(struct Header *)buffer;
//	MemBlank(buffer,4096);
	memcpy(header->version,"CLR B+01",8);
//	header->data=1024;//version sans ID string
//	header->tree=2048;//version sans ID string
	header->data=3072;
	header->tree=4096;
	header->tree_nb_sheet=4;

	header->data_last_element=0;

/*	for(comp=0;comp<15;comp++)
	{
		printf("AddData %d\n",BTreeDataInsert(4,"ABCD"));
	}
*/
//	printf("delete4 %lu\n",BTreeDataDelete(4));
//	printf("AddData %lu\n",BTreeDataInsert(4,"cd04"));
//	header->tree_entry_point=BTreeBuild(header->tree_nb_sheet);
/*	for(comp=0;comp<33;comp++)
	{
		printf("Find %d\n",comp);
		dt=BTreeSearch(comp);
		if(error)
		{
			printf("Data %d not found\n",comp);
		}
		else
		{
			printf("Data %d is: %s\n",comp,dt->value);
		}
	}
*/
   comp = strspn("ABCDEFG019874", "ABCD");
   printf("Length of initial segment matching %d\n", comp );
    comp = strspn("ABCDEFG019874", "ABCDE");
   printf("Length of initial segment matching %d\n", comp );
    comp = strspn("ABCDEFG019874", "BCD");
   printf("Length of initial segment matching %d\n", comp );

   comp = strcmp("ABCD", "ABCC");
   printf("Length of initial segment matching %d\n", comp );
    comp = strcmp("ABCD", "ABCD");
   printf("Length of initial segment matching %d\n", comp );
    comp = strcmp("ABCD", "ABCE");
   printf("Length of initial segment matching %d\n", comp );
 
	for(comp=0;comp<2;comp++)
	{
		BTreeDataWstringInsert((ulong)4,(char*)"EFGH",(ulong)4,(char*)"ABCD");
		printf("AddData %d error=%d\n",comp,error);
	}


	lseek(disk,0,SEEK_SET);
	write(disk,buffer,4096);
	close(disk);
	free (buffer);
	return 0;
}
struct Data* BTreeSearch(ulong ID)//yes
{
	ulong comp,iter;
	struct Element *el;
	el=(struct Element *)(buffer+header->tree);

	comp=header->tree_entry_point;

	error=0;
	//pas d'arbre
	if(el[comp].ID==0)
	{
		printf("No B+ Tree structure found at %lu\n",comp);
		error=1;
		return 0;
	}

	iter=0;
	while(el[comp].ID)
	{
		iter++;
		printf("iter %lu",iter);
//printf("ID %lx elID %lx comp %lu\n",ID,el[comp].ID,comp);
		if (ID<el[comp].ID)//branche inférieure
		{
			if(el[comp].major)
			{
//printf("ID<el %lu jump to minor %lu\n",comp,el[comp].minor);
				comp=el[comp].minor;
			}
			else//derniere feuille
			{
				error=1;
				//printf("Not found2\n");
				return (struct Data *)0;
			}
		}
		else if(ID==el[comp].ID)//trouvé
		{
			//printf("Found!\n");
			return (struct Data *)(el[comp].data+(buffer+header->data));
		}
		else if(ID>el[comp].ID)
		{
//printf("aaaacomp %lx next %lx ID %lx\n",comp,el[comp].next,el[comp].ID);
//printf("aaaaminor %lx maj %lx data %lu\n",el[comp].minor,el[comp].major,el[comp].data);
			if(el[comp].next)
			{
//printf("ID %lx,el[comp].ID %lx, el[el[comp].next].ID %lx,el[comp].major %lx\n",ID,el[comp].ID,el[el[comp].next].ID,el[comp].major);
				if((ID>el[el[comp].next].ID))// & (el[comp].major==0))
				{
//printf("ID>el %lx jump to next %lx\n",comp,el[comp].next);
					comp=el[comp].next;
				}
				else if(ID==el[el[comp].next].ID)
				{
					//printf("Found!\n");
					return (struct Data *)(el[el[comp].next].data+(buffer+header->data));
				}
				else
				{
//printf("ID>el %lx jump to major %lx\n",comp,el[comp].major);
					if(el[comp].major)
					{
						comp=el[comp].major;
					}
					else
					{
						error=1;
						//printf("Not found3\n");
						return (struct Data *)0;
					}
				}
			}
			else//derniere feuille
			{
				if(el[comp].major)
				{
					comp=el[comp].major;
				}
				else
				{
//printf("ID>el %lx jump to next %lx %lx\n",comp,el[comp].next,el[comp].ID);
					error=1;
					//printf("Not found3\n");
					return (struct Data *)0;
				}
			}
		}
	}
//	printf("Not Found\n");
	error=1;
	return 0;
}
struct Data* BTreePlusSearch(ulong ID)//yes
{
	ulong comp;
	struct ElementPlus *el;
	el=(struct ElementPlus *)(buffer+header->tree);

	comp=header->tree_entry_point;

	error=0;
	//pas d'arbre
	if(el[comp].ID==0)
	{
		printf("No B+ Tree structure found at %lu\n",comp);
		error=1;
		return 0;
	}

	while(1)
	{
//printf("ID %lu elID %lu comp %lu\n",ID,el[comp].ID,comp);
		if(el[comp].ID==0)//fin de branche
		{
			if(el[comp-1].major)
			{
				comp=el[comp-1].major;
			}
			else//derniere feuille
			{
				printf("Not Found\n");
				error=1;
				return 0;
			}
		}
		else if (ID<el[comp].ID)//branche inférieure
		{
			if(el[comp].major)
			{
				comp=el[comp].minor;
			}
			else//derniere feuille
			{
				error=1;
				printf("Not found2\n");
				return (struct Data *)0;
			}
			//comp++;
		}
		else if(ID==el[comp].ID)
		{
			if(el[comp].major)
			{
				comp=el[comp].major;
			}
			else//derniere feuille
			{
				printf("Found!\n");
				return (struct Data *)(el[comp].minor+(buffer+header->data));
			}
		}
		else if(ID>el[comp].ID)
		{
			comp++;
		}
	}
}

ulong BTreeBuild(ulong nb_sheet)//yes
{
	ulong minor,comp_l1,start_l1,last_l1,last_l2,nb_l1,nb_l2;
	struct Data *dt;
	struct Element *el;
	dt=(struct Data *)(buffer+header->data);
	el=(struct Element *)(buffer+header->tree);

	if(nb_sheet<3)//absurd value
	{
		error=1;
		printf("BTreeBuild: Absurd nb_sheet value %lu\n",nb_sheet);
		return 0;
	}
	//ajout des elements de l'arbre
	for(comp_l1=0;dt->next;comp_l1++)
	{
//printf("AZERT %d\n",comp_l1);
		el[comp_l1].ID=dt->ID;
		el[comp_l1].minor=0;
		el[comp_l1].major=0;
		el[comp_l1].data=((void*)dt-(void*)buffer-header->data);
		el[comp_l1].next=comp_l1+1;
		el[comp_l1].rempliss1=-1;
		el[comp_l1].rempliss2=-1;
//			el[comp_l1].minor=((void*)last-(void*)buffer-header->data);
//			el[comp_l1].major=((void*)dt-(void*)buffer-header->data);
//printf("Add element %d %d %d\n",dt->ID,el[comp_l1].minor,el[comp_l1].major);
		dt=(struct Data *)(buffer+header->data+dt->next);
	}
	el[comp_l1].ID=dt->ID;
	el[comp_l1].minor=0;
	el[comp_l1].major=0;
	el[comp_l1].data=((void*)dt-(void*)buffer-header->data);
	el[comp_l1].next=0;
	el[comp_l1].rempliss1=-1;
	el[comp_l1].rempliss2=-1;
	comp_l1++;
	el[comp_l1].ID=0;
	el[comp_l1].minor=0;
	el[comp_l1].major=0;
	el[comp_l1].next=0;
	el[comp_l1].data=0;
	el[comp_l1].rempliss1=-1;
	el[comp_l1].rempliss2=-1;
	comp_l1++;

	//montage de l'arbre
	comp_l1=0;
	start_l1=0;
	do{
		nb_l1=0;
		nb_l2=0;
		last_l1=0;
		last_l2=0;
		minor=start_l1;
//printf("Avant For %lu %lx\n",start_l1,el[start_l1].ID);
		for(comp_l1=start_l1;el[comp_l1].next;)
		{
//printf("For %lx %lx\n",comp_l1,el[comp_l1].ID);
			nb_l1++;
			if((nb_l1%nb_sheet==0))
			{
//printf("Change element %lx %lx %lx %lx %lx %lx\n",start_l1,comp_l1,last_l1,last_l2,nb_l1,nb_l2);
				if(nb_l2==0)
				{
					start_l1=comp_l1;
				}
				else
				{
//printf("last2 %lx %lx %lx\n",last_l2,el[last_l2].major,el[last_l2].next);
					el[last_l2].next=comp_l1;
				}
				el[last_l1].next=el[comp_l1].next;
				//el[last_l1].major=el[comp_l1].minor;//el[comp_l1].next;
				el[comp_l1].minor=minor;
				el[comp_l1].major=el[comp_l1].next;
				el[comp_l1].next=0;

				last_l2=comp_l1;
				comp_l1=el[comp_l1].major;

				nb_l2++;
				minor=comp_l1;
			}
			else
			{
				last_l1=comp_l1;
				comp_l1=el[comp_l1].next;
			}
		}
	}while(nb_l2>nb_sheet);

	//header->tree_entry_point=start_l1;
	return start_l1;
//	printf("Start %lx\n",start_l1);
}

ulong BTreePlusBuild(ulong nb_sheet)//yes
{
	ulong comp,comp2,comp3,comp4,start1,start2,minor;
	struct Data *dt;
	struct ElementPlus *el;
	dt=(struct Data *)(buffer+header->data);
	el=(struct ElementPlus *)(buffer+header->tree);

	if(nb_sheet<3)//absurd value
	{
		error=1;
		printf("BTreeBuild: Absurd nb_sheet value %lu\n",nb_sheet);
		return 0;
	}
	//ajout des elements de l'arbre
	for(comp=0;dt->next;comp++)
	{
//printf("AZERT %d\n",comp);
		el[comp].ID=dt->ID;
		el[comp].minor=((void*)dt-(void*)buffer-header->data);
		el[comp].major=0;
//			el[comp].minor=((void*)last-(void*)buffer-header->data);
//			el[comp].major=((void*)dt-(void*)buffer-header->data);
//printf("Add element %d %d %d\n",dt->ID,el[comp].minor,el[comp].major);
		dt=(struct Data *)(buffer+header->data+dt->next);
	}
	el[comp].ID=dt->ID;
	el[comp].minor=((void*)dt-(void*)buffer-header->data);
	el[comp].major=0;
	comp++;
	el[comp].ID=0;
	el[comp].minor=0;
	el[comp].major=0;
	comp++;

	//montage de l'arbre
	start1=0;
	comp2=comp;
	do
	{
		comp3=0;
		comp4=0;
		minor=start1;
		start2=comp2;
//printf("For %d %d\n",start1,el[start1].ID);
		for(comp=start1;el[comp].ID;comp++)
		{
//printf("For %d %d\n",comp,el[comp].ID);
			if((comp4%header->tree_nb_sheet==0) & (comp4!=0))
			{
//printf("Add element %lx %lu %lu %lu %lu %lx %lx\n",el[comp].ID,comp,comp2,comp3,comp4,start1,start2);
				el[comp2].ID=el[comp].ID;
				el[comp2].minor=minor;
				el[comp2].major=comp;
				minor=comp;
				comp2++;
				comp3++;
			}
			comp4++;
		}
		el[comp2].ID=0;
		el[comp2].minor=0;
		el[comp2].major=0;
		comp2++;
		if(comp3>header->tree_nb_sheet)
		{
			start1=start2;
		}
	}while(comp3>header->tree_nb_sheet);

//	header->tree_entry_point=start1;
	return start1;
//	printf("Start %lu\n",start1);
}

ulong BTreeDataInsert(ulong size, char* value)//yes
{
	struct Data *dt, *last, *temp;

	dt=(struct Data *)(buffer+header->data);

//	printf("buffer %p\n",buffer);

	temp=(struct Data *)0;
	do//boucle pour trouver un trou ou le dernier de la chaine
	{
		last=dt;
		dt=(struct Data *)(buffer+header->data+last->next);
//printf("do while %d %d %d %d\n",dt,buffer,dt->next,last->next);
		if(last->ID+1!=dt->ID)//trou détecté donc valeur inséré ici
		{
//printf("trou\n");
			temp=last;
			break;
		}
	}while(dt->next);
	if(temp)
	{
		last=temp;
	}
	else{last=dt;}

	dt=(struct Data *)(buffer
	+header->data
	+header->data_last_element);
//printf("dt %p\n",dt);

	dt->ID=last->ID+1;
	dt->next=last->next;
	last->next=((void*)dt-(void*)buffer-header->data);
//printf("last %p %d last_element:%p\n",last,header->data,header->data_last_element);
	dt->size=size;
	memcpy(dt->value,value,size);
//	printf("%s\n",dt->value);
	header->data_last_element+=size+sizeof(struct Data);
	return dt->ID;
}

void BTreeDataWstringInsert(ulong size, char* ID, ulong size_value, char* value)//no
{
	struct DataWString *dt, *last, *temp;
	ulong comp,comp2;

	error=0;
	dt=(struct DataWString *)(buffer+header->data);

//	printf("buffer %p\n",buffer);

	temp=(struct DataWString *)0;
	while(dt->size)//boucle pour trouver sa place
	{
		last=dt;
		dt=(struct DataWString *)(buffer+header->data+last->next);
printf("do while %d %d %d %d\n",comp,buffer,size,dt->size);
		if(size<=dt->size)
		{
			comp=size;
		}
		else
		{
			comp=dt->size;
		}
		comp2=strncmp(ID,dt->data,comp);
		if(comp2<0)//ID après dt
		{
printf("trou\n");
			temp=last;
			break;
		}
		else if (comp2==0)//identique donc interdit
		{
printf("comp2=0\n");
			error=1;
			return;
		}
	}

	if(temp){last=temp;}
	else{last=dt;}

	dt=(struct DataWString *)(buffer
	+header->data
	+header->data_last_element);
printf("dt %p\n",dt);

	last->next=((void*)dt-(void*)buffer-header->data);

	memcpy(dt->data,ID,size);
	dt->next=last->next;
	dt->size=size;
printf("last %p %d last_element:%p\n",last,header->data,header->data_last_element);

	memcpy(dt->data+size,value,size_value);
	dt->size_value=size_value;
printf("%s\n",dt->data+size);
	header->data_last_element+=size+size_value+sizeof(struct Data);
	return;
}

ulong BTreeDataDelete(ulong ID)//yes
{
	struct Data *dt, *last;

	dt=(struct Data *)(buffer+header->data);

//printf("buffer %d\n",buffer);

	do
	{
		last=dt;
		dt=(struct Data *)(buffer+header->data+last->next);
//printf("do while %d %d %d %d\n",dt,buffer,dt->next,last->next);
		if(dt->ID==ID)//trouvé
		{
			dt->ID=0;
			last->next=dt->next;
			dt->next=0;
//printf("next %d %d %d\n",dt,buffer,last->next);
			return ID;
		}
	}while(dt->next);

	//pas trouvé
	return 0;
}

void BTreeDataUpdate(ulong ID, ulong size, char* value)//no
{
	ulong *last_data;
	struct Data *dt;

	dt=(struct Data *)(buffer+1024);

//	printf("buffer %d\n",buffer);
	last_data=(ulong*)dt;
//	printf("last data %d\n",*last_data);

	dt=(struct Data *)(buffer+1024+*last_data);
//	printf("data %d\n",dt);

	dt->ID=ID;
	dt->size=size;
	memcpy(dt->value,value,size);
	*last_data+=size+sizeof(struct Data);
//	printf("%s\n",dt->value);
}

struct Data* BTreeDataFind(ulong offset,ulong ID)//ok
{
	struct Data *dt, *last;

	dt=(struct Data *)(buffer+header->data+offset);
	error=0;

//printf("buffer %d\n",buffer);

	do
	{
		last=dt;
		dt=(struct Data *)(buffer+header->data+last->next);
//printf("do while %d %d %d %d\n",dt,buffer,dt->next,last->next);
		if(dt->ID==ID)//trouvé
		{
//printf("next %d %d %d\n",dt,buffer,last->next);
			error=0;
			return dt;
		}
	}while(dt->next);

	//pas trouvé
	error=1;
	return dt;
}

void MemBlank(char* buff, ulong size)//ok
{
	ulong comp;
	for (comp=0;comp<size; comp++)
	{
		buff[comp] = '\0';
	}
}
