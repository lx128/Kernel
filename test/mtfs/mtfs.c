/*
Driver MTFS Version 0
*/
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


#include"mtfs.h"

uint disk, disk_size;
ushort sector_size;
ushort cluster_size;
ushort cluster_size_byte;
ushort ENTRY_SIZE;

uint mt, mt_size;
uint mt_nb_of_entry, mt_nb_of_entry_per_cluster;
uint mt_bitmap, mt_bitmap_size;
uint data_bitmap, data_bitmap_size;
uchar config[16], config_nb;

int main()
{
	uint comp;
	disk_size=1048576;
	sector_size=512;
	cluster_size=8;
	cluster_size_byte=4096;
	ENTRY_SIZE=ENTRY_SIZE_FS;
	uchar buff[96]="ZZZsfdgsdfgsdfgserazwcxv3132156465ffffffffffffgggggggggggggggggggghhhhhhhhhhhhhhhhhhhhjjjjjjjjjj";

	disk=open("disk.bin",O_CREAT|O_RDWR,0666);
printf("%s\n",strerror(errno));

	MTFSFormat(cluster_size);//taille cluster
/*	MTFSInit();
	file=(string*)buff;
	file->size=18;
	memcpy(file->data,"s\0a\0l\0u\0t\0.\0e\0x\0e\0",file->size);
	CreateFile(file);*/

//	MTBitmapWrite(8191, 1, 1);
//	MTBitmapWrite(8190, 1, 1);

/*	printf("Compare %u\n",BitmapMatch(mt_bitmap, mt_bitmap_size,31, 3, 1));
	printf("Compare %u\n",BitmapMatch(mt_bitmap, mt_bitmap_size+1,32767, 3, 0));
	printf("Compare %u\n",BitmapMatch(mt_bitmap, mt_bitmap_size,160, 32608, 1));
	BitmapWrite(mt_bitmap, mt_bitmap_size+1,32895, 3, 0);

	comp=BitmapSearch(mt_bitmap, mt_bitmap_size+1,32767, 12, 0);
	printf("Search0 %u\n",comp);
	BitmapWrite(mt_bitmap, mt_bitmap_size+1,comp, 12, 1);

	comp=BitmapSearch(mt_bitmap, mt_bitmap_size+1,119, 33, 1);
	printf("Search1 %u\n",comp);
	BitmapWrite(mt_bitmap, mt_bitmap_size+1,comp, 32, 0);
*/
//	EntryWrite(buff,31,31);
/*strcpy(buff,"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
	EntryRead(buff,95,127);
	printbuff(buff,95);

	EntryDelete(1,1);*/
printf("%d %d %d %d %d %d\n",mt,mt_size,mt_bitmap,mt_bitmap_size,data_bitmap, data_bitmap_size);
//return 0;

	close(disk);
	return 0;
}

void Dispatch(void* buff,uchar nb,uchar com,string* data, uint datapos)//no
{
	switch(nb)
	{
		case PNAME:
			switch(com)
			{
				case ADD:
					//return NomAdd(buff,data);
				break;
				case READ:
					//return NomRead(buff,data,datapos);
				break;
				case SIZE:
				break;
			}
		break;
		case PDATA:
			switch(com)
			{
				case ADD:
					//DataAdd(buff,data,datapos);
				break;
				case READ:
				break;
				case SIZE:
				break;
			}
		break;
	}
}

int MTFSFormat(ushort cluster_size2)//no
{
//Création fichier $MT
//Création fichier $MT2
//Création fichier $BOOT
//Création fichier $MTFREE
	uchar *buff, name[32];
	uint cat_size, offset;
	string *sname;
	ullong comp;
	struct BootBlock *boot;
	struct Config *config;
	struct MTEntry *entry;
	struct Property *prop;

	cluster_size=cluster_size2;
	cluster_size_byte=cluster_size2*sector_size;
	mt_nb_of_entry_per_cluster=cluster_size_byte/ENTRY_SIZE;

	mt=2;
	mt_size=disk_size/cluster_size_byte/10;//10% du disque
	mt_nb_of_entry=mt_size*mt_nb_of_entry_per_cluster;

	mt_bitmap=mt+mt_size;
	mt_bitmap_size=mt_size*mt_nb_of_entry_per_cluster/cluster_size_byte+1;

	data_bitmap=mt_bitmap+mt_bitmap_size;
	data_bitmap_size=mt_bitmap_size*mt_nb_of_entry_per_cluster/cluster_size_byte+1;
printf("ZZZ %u %u\n",mt,mt_size);
printf("EEE %u %u\n",mt_bitmap,mt_bitmap_size);
printf("RRR %u %u\n",data_bitmap,data_bitmap_size);

	sname=(string*)name;
	buff=malloc(cluster_size_byte);
	MemBlank(buff,cluster_size_byte);

	//boot block
	BitmapWrite(data_bitmap, data_bitmap_size,0, 1, 1);
	boot=(struct BootBlock*)buff;
	memcpy(boot->signature,"MTFS32 CLR",10);
	boot->config=1;
	DWrite(buff,0,cluster_size);

	//Config block
	BitmapWrite(data_bitmap, data_bitmap_size,1, 1, 1);
	MemBlank(buff,cluster_size_byte);
	config=(struct Config*)buff;
	config->version=0;
	config->sector_size=sector_size;
	config->cluster_size=cluster_size_byte;
	config->mt=mt;
	config->mt_size=mt_size;
	config->mt_mir=0;
	config->mt_mir_size=0;
	DWrite(buff,cluster_size,cluster_size);
	free(buff);

//Création fichier $MT
	//Init entry
	BitmapWrite(mt_bitmap, mt_bitmap_size,0, 1, 1);
	buff=malloc(ENTRY_SIZE*15);
	offset=0;//Pointeur dans l'entry
	MemBlank(buff,ENTRY_SIZE);
	entry=(struct MTEntry*)buff;
	entry->type=MTFILE;
	entry->previous=0;
	entry->next=0;

	//ajout nom
	memcpy(sname->data,"$MT",3);
	sname->size=3;
//printf("%s\n",sname->data);
	offset+=PropAdd(PNAME,buff,sname);

	//ajout clusters
	comp=disk_size/cluster_size_byte/10;
	sname->size=DataRunAdd(sname->data,1,comp);
	offset+=PropAdd(PDATA_NR,buff+offset,sname);
	BitmapWrite(data_bitmap, data_bitmap_size,2, comp, 1);
//printf("size mt %d\n",comp);

	//ajout taille
	comp=disk_size/10;
	memcpy(sname->data,&comp,8);
	sname->size=8;
	offset+=PropAdd(PINFO,buff+offset,sname);

	EntryWrite(buff,offset,0);

//Création fichier $MTFREE
//Champ de bit 0=libre 1=pris
	BitmapWrite(mt_bitmap, mt_bitmap_size,1, 1, 1);
	offset=0;//Pointeur dans l'entry
	MemBlank(buff,ENTRY_SIZE);
	entry=(struct MTEntry*)buff;
	entry->type=MTFILE;
	entry->previous=0;
	entry->next=0;

	//ajout nom
	memcpy(sname->data,"$MTFREE",7);
	sname->size=7;
//printf("%s\n",sname->data);
	offset+=PropAdd(PNAME,buff,sname);

	//ajout clusters
	sname->size=DataRunAdd(sname->data,1,(disk_size/cluster_size_byte/10));
	offset+=PropAdd(PDATA_NR,buff+offset,sname);
	BitmapWrite(data_bitmap, data_bitmap_size,2, disk_size/cluster_size_byte/10, 1);
//printf("size mtfree %d\n",disk_size/cluster_size_byte/10);

	//ajout taille
	comp=disk_size/10;
	memcpy(sname->data,&comp,8);
	sname->size=8;
	offset+=PropAdd(PINFO,buff+offset,sname);

	EntryWrite(buff,offset,16);
//	MTInit();
return 0;

//Création fichier $Boot
	MemBlank(buff,ENTRY_SIZE);
	entry=(struct MTEntry*)buff;
printf("entry %d\n",(int)entry);
	entry->type=2;
	//entry->size=sizeof(struct MTEntry);

	//ajout nom
	memcpy(sname->data,"$Boot",5);
	sname->size=5;
//printf("%s\n",sname->data);
	//NomAdd(buff,sname);

	//ajout clusters
	//prop=GetAttribOffset(buff+sizeof(struct MTEntry),PEXTDATA);
	//prop->type=PEXTDATA;
	//comp=DataRunAdd((struct DataRun*)prop->data,0,1);
	prop->size=comp+PROPERTY_SIZE;
	//entry->size+=prop->size;

	//ajout taille
//	PropAdd(SIZE,buff,cluster_size_byte);

//Création fichier $Bblocks Cluster allocation table
	entry=(struct MTEntry*)(buff+ENTRY_SIZE);
	entry->type=2;
	//entry->size=sizeof(struct MTEntry);
	//ajout nom
	memcpy(sname->data,"$Bblocks",8);
	sname->size=8;
printf("%s\n",sname->data);
	//NomAdd((char*)entry,sname);
//printf("%d\n",entry->size);
printf("entry %d\n",(int)entry);
	DWrite(buff,mt,2);

	//ajout clusters
	//prop=GetAttribOffset(buff+sizeof(struct MTEntry),PEXTDATA);
	//prop->type=PEXTDATA;
	//comp=DataRunAdd((struct DataRun*)prop->data,0,1);
	prop->size=comp+PROPERTY_SIZE;
	//entry->size+=prop->size;

	//ajout taille
//	PropAdd(SIZE,buff,1);//1 cluster

//Création fichier $CAT Cluster allocation table
	MemBlank(buff,ENTRY_SIZE);
	entry=(struct MTEntry*)buff;
	entry->type=2;
	//entry->size=sizeof(struct MTEntry);
	cat_size=disk_size/(cluster_size_byte*sector_size)/8;

	//ajout nom
	memcpy(sname->data,"$CAT",4);
	sname->size=4;
printf("%s\n",sname->data);
	//NomAdd(buff,sname);

	//ajout clusters
	comp=disk_size/cat_size/8/cat_size;
	if (comp==0)
	{
		comp=1;
	}
	//ExtDataAdd(buff,comp);//disk

	//ajout taille
	//PropAdd(SIZE,buff,comp*cat_size);

	//DWrite(buff,mt+(entry_size_sect*2),entry_size_sect);

//exit(0);
/*	PreallocToFile(sname,40);

	//Création fichier $CONFIG
	memcpy(sname->data,"$\0C\0O\0N\0F\0I\0G\0",14);
	sname->size=14;
	CreateFile(sname,0);
printf("%d\n",config[2]);*/
	free(buff);
}/*
int MTFSInit()//ok
{
	char buff[ENTRY_SIZE], nom[20], *data;
	uint *integer;
	ullong *size;
	struct BootBlock *boot;
	string *test;

	DRead(buff,0,1);
	boot = (struct BootBlock*)buff;
	if (strncmp(boot->signature,"MTFS32 CLR",sizeof(boot->signature))!=0 || boot->version!=0)
	{
		printf("Erreur: Partition non reconnu.\n");
		return -1;
	}
	memcpy(&info, buff + sizeof(struct BootBlock), sizeof(struct BootBlockV0));
	entry_lng_sect=ENTRY_SIZE/info.sector_size;

	mt_start=info.offset;

	DRead(buff,mt_start,entry_lng_sect);
	integer=(uint*)(GetAttribOffset(buff,SIZE)+buff);
	if (integer==0){printf("Erreur dans le fichier $MT.\n");return -1;};
	mt_size=*integer;

	test=(string*)(GetAttribOffset(buff,NAME)+buff);
	test=(struct string*)nom;
	memcpy(test->data,"$\0C\0O\0N\0F\0I\0G\0",14);
	test->size=14;
	if (SelectMasterFile(test,buff)==-1){printf("Erreur: Fichier $CONFIG introuvable.\n");return -1;}
	data=(char*)(GetAttribOffset(buff,DATA)+buff);
	size=(ullong*)(GetAttribOffset(buff,SIZE)+buff);
	config_nb=*size;
	memcpy(config,data,*size);
}*/

struct Property* GetAttribOffset(void* buff,uchar attr)//ok
{
	uchar comp;
	struct Property *prop;
	prop=(struct Property*) buff;

	for(comp=0;prop->size!=0;comp++)
	{
		if(prop->type==attr)//Attribut trouvé
		{
printf("GetAttribOffset: Attribut deja renseigne.\n");
			return prop;
		}
		prop= (struct Property*)((char*)prop+prop->size);
	}
printf("GetAttribOffset: Attribut non renseigne.\n");
	return prop;
}

ushort PropAdd(uchar type, void* buff,string* data)//no
{
	struct Property *prop;

	prop=(struct Property*)buff;
	prop->type=type;
	prop->size=data->size+PROPERTY_SIZE;
	memcpy(prop->data,data->data,data->size);
printf("e%de\n",type);

	return prop->size;
}

//retourne 0 si echec, type si succes
uchar PropRead(uchar type,void* buff, string* data)//no
{
	struct Property *prop;
	prop = (struct Property*)buff;

	while(prop->type!=0)
	{
		if(prop->type==type)//Attribut trouvé
		{
printf("GetAttribOffset: Attribut trouvé.\n");
			data->size=prop->size-PROPERTY_SIZE;
			memcpy(data->data,prop->data,data->size);
			return type;
		}
		prop= (struct Property*)((uchar*)prop+prop->size);
	}
printf("GetAttribOffset: Attribut non renseigne.\n");
	return 0;
}

int DataRunAdd(char *data,long start, long size)//ok
{
	struct DataRun *dt;
	dt=(struct DataRun *) data;

	if(start<0x100)
	{
		dt->offset_field_size=1;
		memcpy(dt->data,&start,1);
	}
	else if(start<0x10000)
	{
		dt->offset_field_size=2;
		memcpy(dt->data,&start,2);
	}
	else if(start<0x1000000)
	{
		dt->offset_field_size=3;
		memcpy(dt->data,&start,3);
	}
	else
	{
		dt->offset_field_size=4;
		memcpy(dt->data,&start,4);
	}
	if(size<0x100)
	{
		dt->lengh_field_size=1;
		memcpy(dt->data+dt->offset_field_size,&size,1);
	}
	else if(size<0x10000)
	{
		dt->lengh_field_size=2;
		memcpy(dt->data+dt->offset_field_size,&size,2);
	}
	else if(size<0x1000000)
	{
		dt->lengh_field_size=3;
		memcpy(dt->data+dt->offset_field_size,&size,3);
	}
	else
	{
		dt->lengh_field_size=4;
		memcpy(dt->data,&start,4);
	}
	return dt->lengh_field_size+dt->offset_field_size+1;
}

/*
uint GetAttribOffset(void *buff,uchar attr)//no
{
	uint comp;
	struct MTEntryV0 *entry;

	entry = (struct MTEntryV0*)buff;
//printf("%d\n",entry->nb);
	for(comp=0;comp<entry->nb;comp++)
	{
		if(entry->prop[comp].type==attr)
		{
			if(attr<128&&entry->prop[comp].add>ENTRY_SIZE)//attribut interne
			{
				printf("Erreur: L'attribut est en dehors de l'index.\n");
				return 0;
			}
			else if(attr<128)
			{
				return entry->prop[comp].add;
			}
		}
	}
	return 0;
}

uint GetAttribSize(void *buff,uchar attr)//no
{
	uint comp;
	struct MTEntryV0 *entry;

	entry = (struct MTEntryV0*)buff;
	for(comp=0;comp<entry->nb;comp++)
	{
		if(entry->prop[comp].type==attr)
		{
			return entry->prop[comp].size;
		}
	}
	return 0;
}

uchar AddAttrib(void *buff,uchar attr,uint add)//no
{
	uint comp;
	struct MTEntryV0 *entry;

	entry = (struct MTEntryV0*)buff;
//printf("%d\n",entry->nb);

	for(comp=0;comp<entry->nb;comp++)//recherche si l'attribut est gere
	{
		if (config[comp]==attr)
		{
			break;
		}
	}
	if(comp>entry->nb)
	{
		printf("Erreur: Attribut non gere.\n");
		return 0;
	}
	if(GetAttribOffset(buff,attr)!=0)//recherche si l'attribut a déjà été renseigné
	{
		printf("Erreur: Attribut deja renseigne.\n");
		return 0;
	}
*/
/*	maxi=2+config_nb*sizeof(struct PropertyV0);
	for(comp=0;entry->nb>comp;comp++)//recherche de la position ou commancer l'ajout
	{
		maxi+=GetAttribSize(buff,config[comp]);
	}
//	entry->prop[comp].type=attr;
//	entry->prop[comp].add=add;
//	entry->prop[res].size=data->size;
//	memcpy(buff+maxi,data->data,data->size);
//	printf("%u\n",maxi);

	return 0;
}
*/
/*
uint SelectIndex(uint index, void *buff)//no
{
	uint size;
	struct MTEntryV0 *entry;

	if(mt_size<(index*entry_lng_sect))
	{
		printf("Erreur: L'index demande est en dehors de la table maitre.\n");
		return 0;
	}

	DRead(buff,mt_start+(index*entry_lng_sect),entry_lng_sect);
	entry = (struct MTEntryV0*)buff;
	if(entry->type>128)
	{
printf("trouve!\n");
		return index;
	}
printf("vide ou effacer!\n");
	return 0;//effacer ou vide
}
*/

int SelectFile(void *buff, string *name)//no
{
	uint size;
	string *name2;
	struct MTEntryV0 *entry;

//	var1=mt_start+16*entry_lng_sect;

//	for(comp1=mt_start;comp1<var1;comp1+=2)
	{
//		DRead(buff,comp1,entry_lng_sect);
		entry = (struct MTEntryV0*)buff;
//		if(entry->type==1 && entry->active==1)
		{
			//NomRead(buff,name2);
			if(name->size>name2->size)
			{
				size=name->size;
			}
			else
			{
				size=name2->size;
			}
			if(memcmp(name2->data,name->data,size)==0)
			{
//printf("trouve! %d\n",name2->size);
//				return comp1-mt_start;
			}
		}
	}
	return -1;
}

int EntrySearchFree(ushort size)//no
{
	uchar buff[ENTRY_SIZE];
	int comp;

	for (comp=size;comp<mt_size;comp++)
	{
//		DRead(buff,mt_start+entry_lng_sect*comp,entry_lng_sect);
		if (buff[0]<128)
		{
			return comp;
		}
	}
	return 0;
}

//
int EntryDelete(ulong entrynb, uchar choice)//yes
{
	uint offset, offset_disk;
	uchar* drive_buff;
	struct MTEntry *entry;

	offset=(entrynb%mt_nb_of_entry_per_cluster)*ENTRY_SIZE;//début dans le buffer
	offset_disk=mt+entrynb/mt_nb_of_entry_per_cluster;

	drive_buff=malloc(cluster_size_byte);

	//lecture de la zone à traité
	DRead(drive_buff,offset_disk,1);

	entry=(struct MTEntry*)drive_buff+offset;
//printf("%d %d\n",entry->erased,choice);
//printbuff(drive_buff,cluster_size_byte);
	if(entry->type!=MTFILE)
	{
		printf("Rien a effacer ou restaurer\n");
		return 0;
	}
	else if(entry->erased==choice)
	{
		printf("Entree deja effacee ou restauree\n");
		return 0;
	}
	else
	{
		printf("Effacement ou restauration de l'entree\n");
		entry->erased=choice;
	}

	DWrite(drive_buff,offset_disk,1);

	free(drive_buff);
	return 0;
}

int EntrySize(ulong entrynb)//no
{
	uint offset, offset_disk;
	uchar* drive_buff;
	struct MTEntry *entry;

	offset=(entrynb%mt_nb_of_entry_per_cluster)*ENTRY_SIZE;//début dans le buffer
	offset_disk=entrynb/mt_nb_of_entry_per_cluster;

	drive_buff=malloc(cluster_size_byte);
	//lecture de la zone à traité
	DRead(drive_buff,mt+offset_disk,1);
	entry=(struct MTEntry*)drive_buff+offset;

	//printf("aaaaaa %d %d %d %d %d\n",drive_buff,*entry,offset,offset_disk,entry->nb);
	//libre ou effacer
//	printbuff(drive_buff,cluster_size_byte);
	if(entry->type==0)
	{
		return 0;
	}

//return(0);
//	free(drive_buff);
	return 0; //entry->nb*ENTRY_SIZE;
}

int EntryRead(uchar* buff, ushort size, ulong entrynb)//no
{
	uint buffer_size, offset, offset_disk, datasize;
	uchar* drive_buff;
	struct MTEntry *entry;

	offset=(entrynb%mt_nb_of_entry_per_cluster)*ENTRY_SIZE;//début dans le buffer
	offset_disk=mt+entrynb/mt_nb_of_entry_per_cluster;

	drive_buff=malloc(cluster_size_byte);


//printbuff(drive_buff,buffer_size);
//printf("%d %d %d\n",entrynb,mt_nb_of_entry,cluster_size_byte);
	//test de limite
	if(entrynb>=mt_nb_of_entry)
	{
		printf("MT demande de lecture hors limite\n");
		return 0;
	}

	//lecture de la zone à traité
	DRead(drive_buff,offset_disk,1);

	entry=(struct MTEntry*)drive_buff+offset;
	if(entry->erased==1)
	{
		printf("Entree effacee\n");
		//return 0;
	}
	if(entry->type!=MTFILE)
	{
		printf("Entree non valide\n");
		return 0;
	}

	datasize=/*entry->nb*/ 0*ENTRY_SIZE-1;
	if(datasize>size)
	{
		printf("Buffer trop petit\n");
		return 0;//buffer cible trop petit
	}

	//taille du buffer
	buffer_size=((offset+datasize)/cluster_size_byte+1)*cluster_size_byte;

	if(buffer_size>cluster_size_byte)
	{
		//allocation du buffer disque
		drive_buff=(uchar*)realloc(drive_buff,buffer_size);

		//Lecture de l'entrée
		DRead(drive_buff+cluster_size_byte,offset_disk+1,buffer_size/cluster_size_byte-1);
	}

	memcpy(buff,entry->data,datasize);

	free(drive_buff);
	return 0;
}

int EntryWrite(uchar* buff, ushort size, ulong entrynb)//yes
{
	uint comp,comp2,buffer_size, offset, offset_disk;
	uchar* drive_buff;
	struct MTEntry *entry;

	//nombre de secteur necessaire
	comp=(size+1)/ENTRY_SIZE;//Longueur + entete(1 octet)
	if((size+1)%ENTRY_SIZE)
	{
		comp++;
	}

	if(comp>15)
	{
		return 0;//entrée trop grande pour être dans la MT
	}
	//test de limite
	comp2=entrynb+comp-1;
//printf("%d %d %d\n",comp2,mt_nb_of_entry,mt+mt_size);
	if(comp2>=mt_nb_of_entry)
	{
		printf("MT demande d'allocation hors limite\n");
		return 0;
	}

	//l'entree ne commence pas forcément au début d'un secteur
	//donc ajout du décalage
	offset=(entrynb%mt_nb_of_entry_per_cluster)*ENTRY_SIZE;//début dans le buffer
	buffer_size=cluster_size_byte;//taille totale du buffer
	offset_disk=(mt+entrynb/mt_nb_of_entry_per_cluster)*cluster_size;
printf("DWRITE 0 %u\n",offset);

	//allocation du buffer disque
	drive_buff=malloc(buffer_size);


	//lecture de la zone à traité
	DRead(drive_buff,offset_disk,cluster_size);
printf("DWRITE 1 %u\n",offset_disk);

	//effacement si une entrée existe déjà
	entry=(struct MTEntry*)drive_buff+offset;
	if(entry->type==MTFILE)
	{
		printf("Effacement ancienne entree\n");
		MemBlank((char*)entry, entry->nb*ENTRY_SIZE);
	}

	//copie de l'entrée à sa place
	entry=(struct MTEntry*)drive_buff+offset;
	entry->type=MTFILE;
	entry->erased=0;
	entry->nb=comp;
	memcpy(entry->data,buff,size);

	//Copie sur le disque
	DWrite(drive_buff,offset_disk,cluster_size);
	free(drive_buff);

//	MTBitmapWrite(entrynb, size, 1);

//printbuff(drive_buff,buffer_size);
//return 0;
	return 0;
}

//bit de depart, nombre à ecrire, 1 ou 0
int BitmapWriteOld(ulong bitmap, ulong bitmap_size, ulong entrynb, ushort size, uchar choice)//yes
{
	uint comp, offset, offsetbyte, start_disk, size_buff;
	uchar* drive_buff;

	comp=cluster_size_byte*8;
	//test limite
//printf("%d %d %d\n",entrynb+size,mt_bitmap_size*comp,size);
/*	if(entrynb+size>bitmap_size*comp || size==0)
	{
		return 0;
	}*/

	offset=(entrynb%comp)/8;//début dans le buffer
	offsetbyte=entrynb%comp%8;//début dans le premier octet
	start_disk=bitmap+entrynb/comp;//cluster concerné
	size_buff=(entrynb%comp+size)/comp;//taille totale du buffer
	if((entrynb%comp+size)%comp)
	{
		size_buff++;
	}

//printf("%d %d %d %d\n",offset,offsetbyte,start_disk,size_buff);
	//allocation du buffer disque
	drive_buff=malloc(size_buff*cluster_size_byte);
	MemBlank(drive_buff,size_buff*cluster_size_byte);

	//lecture de la zone à traité
	DRead(drive_buff,start_disk,cluster_size);

	//changement
	//debut
	if(size>7)
	{
		comp=255;
		size-=8-offsetbyte;
	}
	else
	{
		comp=pow(2,size%8)-1;
		size=0;
	}

	if(choice==1)
	{	drive_buff[offset]=drive_buff[offset]|
		comp<<offsetbyte;
	}
	else
	{	drive_buff[offset]=drive_buff[offset]&
		(255-(comp<<offsetbyte));
	}

//printf("aaa %d\n",drive_buff[offset]);
	//millieu
	for(comp=1;size>7;comp++)
	{
		if(choice==1)
		{	drive_buff[offset+comp]=255;
		}
		else
		{	drive_buff[offset+comp]=0;
		}
		size-=8;
//printf("bbb 255 %d\n",size);
	}

	//fin
	if(size!=0)
	{
		if(choice==1)
		{	drive_buff[offset+comp]=drive_buff[offset+comp]|
			(char)(pow(2,size)-1);
		}
		else
		{	drive_buff[offset+comp]=drive_buff[offset+comp]&
			(char)(255-(pow(2,size)-1));
		}
	}

//printf("ccc %d %d %d %d %d\n",offset,comp,size,buffer_size,drive_buff[offset/8+comp]);

	//Copie sur le disque
	DWrite(drive_buff,start_disk,size_buff);

	free(drive_buff);
	return 0;
}

int BitmapMatchOld(ulong bitmap, ulong bitmap_size, ulong entrynb, ushort size, uchar choice)//yes
{
	uint comp, offset, offsetbyte, start_disk, size_buff;
	uchar* drive_buff;

	comp=cluster_size_byte*8;
	//test limite
//printf("%d %d %d\n",entrynb+size,mt_bitmap_size*comp,size);
/*	if(entrynb+size>bitmap_size*comp || size==0)
	{
		return 0;
	}*/

	offset=(entrynb%comp)/8;//début dans le buffer
	offsetbyte=entrynb%comp%8;//début dans le premier octet
	start_disk=bitmap+entrynb/comp;//cluster concerné
	size_buff=(entrynb%comp+size)/comp;//taille totale du buffer
	if((entrynb%comp+size)%comp)
	{
		size_buff++;
	}

//printf("%d %d %d %d\n",offset,offsetbyte,start_disk,size_buff);
	//allocation du buffer disque
	drive_buff=malloc(size_buff*cluster_size_byte);

	//lecture de la zone à traité
	DRead(drive_buff,start_disk,size_buff);

	//changement
	//debut
	if(size>7)
	{
		comp=255;
		size-=8-offsetbyte;
	}
	else
	{
		comp=pow(2,size%8)-1;
		size=0;
	}

	if(choice==1)
	{	if(drive_buff[offset]!=(drive_buff[offset]|
		comp<<offsetbyte)){return 0;}
	}
	else
	{	if(drive_buff[offset]!=(drive_buff[offset]&
		(255-(comp<<offsetbyte)))){return 0;}
	}

//printf("aaa %d\n",drive_buff[offset]);
	//millieu
	for(comp=1;size>7;comp++)
	{
		if(choice==1)
		{	if(drive_buff[offset+comp]!=255){return 0;}
		}
		else
		{	if(drive_buff[offset+comp]!=0){return 0;}
		}
		size-=8;
//printf("bbb 255 %d\n",size);
	}

	//fin
	if(size!=0)
	{
		if(choice==1)
		{	if(drive_buff[offset+comp]!=(drive_buff[offset+comp]|
			(char)(pow(2,size)-1))){return 0;}
		}
		else
		{	if(drive_buff[offset+comp]!=(drive_buff[offset+comp]&
			(char)(255-(pow(2,size)-1)))){return 0;}
		}
	}

//printf("ccc %d %d %d %d %d\n",offset,comp,size,buffer_size,drive_buff[offset/8+comp]);

	free(drive_buff);
	return 1;
}

int BitmapMatch(ulong bitmap, ulong bitmap_size, ulong entrynb, ushort size, uchar choice)//yes
{
	uint a,b,comp, start, current, end, start_disk, buff_size_int,sector_size_int;
	uint* drive_buff;

//printf("Start %u %u\n",bitmap,cluster_size);
//printf("Start %u %u %u\n",entrynb+size,bitmap_size*cluster_size_byte*8, size);
//printf("Start %u\n",entrynb+size>bitmap_size*cluster_size_byte);

	//test limite
	if((entrynb>(bitmap_size*cluster_size_byte*8))|((entrynb+size)>(bitmap_size*cluster_size_byte*8)) | size==0)
	{
		printf("MTFS:Error: Bitmap overrun\n");
		return FALSE;
	}

	buff_size_int=cluster_size_byte/sizeof(uint);//buffer size in uint chunk
	sector_size_int=sector_size/sizeof(uint);//buffer size in uint chunk

	start_disk=bitmap*cluster_size;//Beginning on disk

	//allocation du buffer disque
	drive_buff=malloc(cluster_size_byte);

	start=entrynb/8/sizeof(uint);
	end=(entrynb+size-1)/8/sizeof(uint);
//	if((entrynb+size)%8){end++;}
	current=entrynb/8/cluster_size_byte*cluster_size;

	//lecture de la zone à traité
	DRead(drive_buff,start_disk+current,cluster_size);
//printf("AAA %u %u %u %u\n",entrynb,size,start,end);
//printf("TTT %u %u %u %u\n",bitmap,bitmap_size,start_disk,end);
	for(comp=start;comp<=end;comp++)
//	comp=start;
//	while(comp<end)
	{
		if((comp%buff_size_int==0))
		{
			if(comp!=start)//perf reason
			{
//printf("Update %u %u\n",comp/buff_size_int, comp);

			current+=cluster_size;

			//lecture de la zone à traité
			DRead(drive_buff,start_disk+current,cluster_size);
			}
		}
		if(comp<start){a=0;}
		else if(comp==start){a=0xFFFFFFFF<<entrynb%(sizeof(uint)*8);}
		else {a=0xFFFFFFFF;}

		//bug dans gcc. 0xff>>8 marche mais pas 0xff>>var.
		if(comp<end){b=0xFFFFFFFF;}
		else if(comp==end){
			b=(entrynb+size)%(sizeof(uint)*8);
//			if(b==32){b=0;printf("B 32\n");}
			//else{
			b=0xFFFFFFFF>>((sizeof(uint)*8)-(entrynb+size)%(sizeof(uint)*8));
//			}
//printf("B2 %u %u %u\n",b,~(a&b), comp);
		}

		if(choice==TRUE)
		{
//printf("Yes %x %x %x\n",a,b,(a&b));
//printf("Yes %x %u %u\n",drive_buff[comp%buff_size_int],drive_buff[comp%buff_size_int]&(a&b),0);
			if((a&b)!=(drive_buff[comp%buff_size_int]&(a&b)))
			{return FALSE;}
		}
		else
		{
//printf("No %x %x %x\n",a,b,(a&b));
//printf("No %x %u %u\n",drive_buff[comp%buff_size_int],0,0);
			if((a&b)!=(~drive_buff[comp%buff_size_int]&(a&b)))
			{return FALSE;}
		}
	}
//printf("Bin %u %u %u %u\n",a,b,(a&b), comp);

	free(drive_buff);
	return TRUE;
}
//bit de depart, nombre à ecrire, 1 ou 0
int BitmapWrite(ulong bitmap, ulong bitmap_size, ulong entrynb, ulong size, uchar choice)//yes
{
	uint a,b,comp, start, current, end, start_disk, buff_size_int,sector_size_int;
	uint* drive_buff;

//printf("Start %u %u\n",bitmap,cluster_size);
//printf("Start %u %u %u\n",entrynb+size,bitmap_size*cluster_size_byte*8, size);
//printf("Start %u\n",entrynb+size>bitmap_size*cluster_size_byte);

	//test limite
	if((entrynb>(bitmap_size*cluster_size_byte*8))|((entrynb+size)>(bitmap_size*cluster_size_byte*8)) | size==0)
	{
		printf("MTFS:Error: Bitmap overrun\n");
		return FALSE;
	}

	buff_size_int=cluster_size_byte/sizeof(uint);//buffer size in uint chunk
	sector_size_int=sector_size/sizeof(uint);//buffer size in uint chunk

	start_disk=bitmap*cluster_size;//Beginning on disk

	//allocation du buffer disque
	drive_buff=malloc(cluster_size_byte);

	start=entrynb/8/sizeof(uint);
	end=(entrynb+size-1)/8/sizeof(uint);
//	if((entrynb+size)%8){end++;}
	current=entrynb/8/cluster_size_byte*cluster_size;

	//lecture de la zone à traité
	DRead(drive_buff,start_disk+current,cluster_size);
printf("AAA %u %u %u %u\n",entrynb,size,start,end);
//printf("TTT %u %u %u %u\n",bitmap,bitmap_size,start_disk,end);
	for(comp=start;comp<=end;comp++)
	{
		if((comp%buff_size_int==0))
		{
			if(comp!=start)//perf reason
			{
//printf("Update %u %u %u\n",current,comp/buff_size_int, comp);
				//Copie sur le disque
				DWrite(drive_buff,start_disk+current,cluster_size);

				current+=cluster_size;

				//lecture de la zone à traité
				DRead(drive_buff,start_disk+current,cluster_size);
			}
		}
		if(comp<start){a=0;}
		else if(comp==start){a=0xFFFFFFFF<<entrynb%(sizeof(uint)*8);}
		else {a=0xFFFFFFFF;}

		//bug dans gcc. 0xff>>8 marche mais pas 0xff>>var.
		if(comp<end){b=0xFFFFFFFF;}
		else if(comp==end){
			b=(entrynb+size)%(sizeof(uint)*8);
//			if(b==32){b=0;printf("B 32\n");}
			//else{
			b=0xFFFFFFFF>>((sizeof(uint)*8)-(entrynb+size)%(sizeof(uint)*8));
//			}
//printf("B2 %u %u %u\n",b,~(a&b), comp);
		}

		if(choice==TRUE)
		{
			drive_buff[comp%buff_size_int]=drive_buff[comp%buff_size_int]|(a&b);
//printf("Yes %u %u\n",drive_buff[comp%buff_size_int],comp%buff_size_int);
		}
		else
		{
			drive_buff[comp%buff_size_int]=drive_buff[comp%buff_size_int]&~(a&b);
//printf("No %u %u\n",drive_buff[comp%buff_size_int],comp%buff_size_int);
		}
	}
//printf("Bin %u %u %u %u\n",a,b,(a&b), comp);

	//Copie sur le disque
	DWrite(drive_buff,start_disk+current,cluster_size);

	free(drive_buff);
	return FALSE;
}

int BitmapSearch(ulong bitmap, ulong bitmap_size, ulong entrynb, ushort size, uchar choice)//yes
{
	uint comp,comp2,comp3,comp4,comp5, start, current, end, start_disk, buff_size_int,sector_size_int;
	uint* drive_buff;

	//test limite
	if((entrynb>(bitmap_size*cluster_size_byte*8))|((entrynb+size)>(bitmap_size*cluster_size_byte*8)) | size==0)
	{
		printf("MTFS:Error: Bitmap overrun\n");
		return FALSE;
	}

	buff_size_int=cluster_size_byte/sizeof(uint);//buffer size in uint chunk
	sector_size_int=sector_size/sizeof(uint);//buffer size in uint chunk

	start_disk=bitmap*cluster_size;//Beginning on disk

	//allocation du buffer disque
	drive_buff=malloc(cluster_size_byte);

	start=entrynb/8/sizeof(uint);
	end=bitmap_size*sector_size_int*cluster_size;
	current=entrynb/8/cluster_size_byte*cluster_size;

	//lecture de la zone à traité
	DRead(drive_buff,start_disk+current,cluster_size);

	comp=start;
	comp2=entrynb%(sizeof(uint)*8);
	comp3=0;
	comp4=start;
	comp5=comp2;
	for(comp=start;comp<=end;comp++)
	{
		if(comp%buff_size_int==0)
		{
			if(comp!=start)//perf reason
			{
//printf("Update %d\n",current);
			current+=cluster_size;
			//lecture de la zone à traité
			DRead(drive_buff,start_disk+current,cluster_size);
			}
		}
//printf("Test: %lX\n",drive_buff[comp%buff_size_int]);
		while(comp2<(sizeof(uint)*8))
		{
			if((drive_buff[comp%buff_size_int]& 1<<comp2)!=0 && choice==TRUE)
				{comp3++;}
			else if(((~drive_buff[comp%buff_size_int])& 1<<comp2)!=0 && choice==FALSE)
				{comp3++;}
			else{comp3=0;comp4=comp;comp5=comp2+1;
//printf("Else comp4 %u comp5 %u\n",comp4,comp5);
			}
//printf("Yes %d 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",comp,comp2,comp3,comp4,(drive_buff[comp%buff_size_int]& 1<<comp2),drive_buff[1]);
			if(comp3==size)
			{
				return (comp4*sizeof(uint)*8+comp5);
			}
			comp2++;
		}
		comp2=0;
	}

	free(drive_buff);
	return FALSE;
}

void DRead(void* buff, uint pos, uint size)//ok
{
	lseek(disk,pos*sector_size,SEEK_SET);
//printf("%s\n",strerror(errno));
	read(disk,buff,size*sector_size);
printf("Read %s Sector nb:%d Size in sect:%d\n",(char*)strerror(errno),pos,size);
}

void DWrite(void* buff, uint pos, uint size)//ok
{
	lseek(disk,pos*sector_size,SEEK_SET);
//printf("%s\n",strerror(errno));
	write(disk,buff,size*sector_size);
printf("Write %s Sector nb:%d Size in sect:%d\n",(char*)strerror(errno),pos,size);
}

void MemBlank(char* buff, long size)//ok
{
	long comp;
	for (comp=0;comp<size; comp++)
	{
		buff[comp] = '\0';
	}
}

void printbuff(uchar* buff, uint size)
{
	uint comp;
	uchar c;
	for(comp=0;comp<size;comp++)
	{
		c=buff[comp];
		if(isprint(c)==0)
		{
			c='.';
		}
		putchar (c);
	}
}
