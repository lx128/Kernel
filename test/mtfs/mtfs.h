/*
Modulable Technologie File System CLR 2011

Fichiers réservés (32 maxi):
$BOOT		Zone BootSect et config
$BOOT2		Zone BootSect et config en fin de disque
$MT			Master Table
$MTS		Master Table Save
$MTFREE		Entry Allocation Bitmap
$LDR		Chargeur de démarrage CLDR
$CABM		Cluster Allocation Bitmap
$BBF		Bad Blocks List
$INDX		Arbre de recherche

Un fichier est un Entry dans la MT.
Celui ci contient des Property comme le Nom.

Créer un fichier revient à créer des Property
dans une Entry qui se trouve dans la MT.

Fonctions sur les Property notés Prop*()
Fonctions sur les Entry notés Entry*()
Fonctions sur MT notés MT*()
Fonctions générale notés MTFS*()

*/
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef struct string string;

#define TRUE 1
#define FALSE 0

void Dispatch(void*,uchar,uchar,string*, uint);
struct Property* GetAttribOffset(void*,uchar);
uchar AddAttrib(void *,uchar , string*);
int MTFSFormat(ushort);
int MTInit();
uint SelectIndex(uint, void*);
ushort PropAdd(uchar, void*,string*);

int EntryWrite(uchar*, ushort, ulong);//yes
int EntryRead(uchar*, ushort, ulong);//yes
int EntryDelete(ulong, uchar);//yes
int BitmapWrite(ulong,ulong,ulong, ulong, uchar);//yes
int BitmapWriteOld(ulong,ulong,ulong, ushort, uchar);//yes
int BitmapMatchOld(ulong,ulong,ulong, ushort, uchar);//yes
int BitmapMatch(ulong,ulong,ulong, ushort, uchar);//yes
int BitmapSearch(ulong,ulong,ulong, ushort, uchar);//yes


void printbuff(uchar*, uint);//yes
void MemBlank(char*, long);//yes
void DRead(void*,uint,uint);//yes
void DWrite(void*,uint,uint);//yes

//#define ENTRY_SIZE 512
#define ENTRY_SIZE_FS 512
#define ENTRY_SIZE_BDR 32
#define RESERVED_ENTRY 32
#define PROPERTY_SIZE 3

//Type de MTEntry
#define MTFREE 0
#define MTFILE 1

//commande de la fonction GetAttribOffset et type de Property
#define PFREE 0
#define PNAME 1//résident 512o max
#define PINFO 2//résident 8o
#define PDATA 3//résident 4ko-(place déjà alloué par les autres propriétées) max
#define PDATA_NR 4//non résident

#define P 1//résident 512o max

#define ADD 0
#define READ 1
#define SIZE 2

struct string//secure string
{
	ulong size;
	uchar data[];
}__attribute__((packed));

struct BootBlock
{
	ulong res1;//jmp
	ulong res2;//jmp
	uchar signature[10];
	ulong config;//position of config in sectors
}__attribute__((packed));

struct Config
{
	ulong size;
	uchar version;
	ushort sector_size;//en octets
	uchar cluster_size;//taille en secteurs
//	uchar offset;//offset depuis le début généralement =1 secteur
//	uchar tree_dimention;//Default 2 Dimension de l'arbre B+
	ulong mt;//en clusters
	ulong mt_size;//en clusters
	ulong mt_mir;//en clusters
	ulong mt_mir_size;//en clusters
	ulong max_entry_size;//in entry chunk
}__attribute__((packed));

//first 16 entry reserved. First of them is Master Table
struct MTEntry
{
	uchar type:3;//voir plus haut
	uchar erased:1;
	uchar size:4; //in entry chunk
//	ulong entry_nb;
//	ulong next;
	uchar data[];
}__attribute__((packed));

struct DataRun
{
	uchar offset_field_size:4;
	uchar lengh_field_size:4;
	uchar data[];
}__attribute__((packed));

/*
Clusters list
Le premier element pointe vers l'adresse reelle les suivants sont relatifs au premier et donc signé.

1 nibble = offset field size in byte
1 nibble = lengh field size in byte
*(int*)(offset field size) = offset
*(u int)(lengh field size) = lengh

*/

struct Property//Situe la propriété dans l'entry
{
	uchar type;	//voir plus haut
	ushort size;//taille totale de la propriétée
	uchar data[];
}__attribute__((packed));

struct FILE//Handle de fichier
{
	ulong ID;	//id dans MT
	ushort size;//taille totale de la propriétée
	uchar data[];
}__attribute__((packed));
