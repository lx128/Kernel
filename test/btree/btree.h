/*CLR All rights Reserved*/
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int là où necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//Sécurité                                                      =0

//Type de valeur courante sous windows
//DWORD (32bits) LITTLE_ENDIAN
//chaine caractere
//chaine Hex
//chaine Hex unicode finissant pas 0x0000. Certains mots comme %system% sont remplacé
//chaines multiple Hex unicode finissant pas 0x0000 pour chaque chaine et
//par 0x0000 2 fois pour la derniere chaine.

//Type de valeur mal connu
//REG_FULL_RESOURCE_DESCRIPTOR 16 octets. Interprété par regedit
//REG_RESOURCE_LIST chaine Hex Variable.
//REG_RESOURCE_REQUIREMENTS_LIST chaine Hex Variable.

/*
Fonctionnement:

Plusieurs base existe sous Windows (HKLM HKCU). Le processus suivant se répète pour chaque base.

La base se décompose en une zone contenant des index (clés ou valeur) et une zone de data (valeurs).
Ces zones ont une taille maxi de 8 Mo.
La seconde zone commence 8 Mo après la première.
Une clé contient une ou plusieurs valeurs et un identifiant de sécutité.
Une valeur ne contient que son nom unicode, son type et sa valeur.

Chaque clé pointe vers une liste de pointeur de valeur et de clé.
*/
#define VERSION "CLRBDR01"
#define HEADER_SIZE 16
#define LIMIT 1024

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;

struct string//secure string
{
	ulong size;
	uchar data[];
}__attribute__((packed));

struct Header//début
{
		char version[8];
		ulong data;
		ulong data_last_element;
		ulong tree;
		ulong tree_last_element;
		ulong tree_entry_point;
		uchar tree_nb_sheet;
} __attribute__ ((packed));

struct Element
{
	ulong ID;//
	ulong minor;//branche gauche ou data
	ulong major;//branche droite
	ulong next;
	ulong data;
	long long rempliss1;
	ulong rempliss2;
}__attribute__((packed));

struct ElementPlus
{
	ulong ID;//
	ulong minor;//branche gauche ou data
	ulong major;//branche droite
	ulong remplissage;
//	uchar rempliss2;
}__attribute__((packed));

struct Data
{
	ulong ID;
	ulong next;
	ulong size;
	char value[];	//nom de la valeur ou de la clé
					//liste d'offset de valeur ou de clé dans index
					//Dans ces 2 cas, commence par 2 octets représentant la taille
					//peut être aussi une chaine, la longueur est dans l'index
} __attribute__ ((packed));

struct DataWString
{
	ulong next;
	uchar size;
	ulong size_value;
	char data[];	//nom de la valeur ou de la clé
					//liste d'offset de valeur ou de clé dans index
					//Dans ces 2 cas, commence par 2 octets représentant la taille
					//peut être aussi une chaine, la longueur est dans l'index
} __attribute__ ((packed));

struct ElementWString
{
	ulong minor;//branche gauche ou data
	ulong major;//branche droite
	ulong next;
	ulong data;
	uchar size;
	uchar ID[];
}__attribute__((packed));

struct index //clés ou valeurs
{
		unsigned char type;	//Type
					//0=effacé ou libre
					//1=clé
					//Ce qui suit ne concerne que les valeurs
					//3=liste de pointeur de clé
					//4=Dword stoké directement dans lkey
					//5=Qword stoké directement dans lkey et lvalue
					//6=chaine char Fixe commence par 2 octets représentant la taille
					//7=chaine char variable 0x0000 fin de chaine
					//8=chaine char variable multiple 0x0000 X 2 fin du bloc de chaine multiple
					//9=chaine Binaire commence par 2 octets représentant la taille
					//Dans le cas des types chaines, lkey représente la longueur de la chaine
					//et lvalue l'adresse de la chaine
		unsigned int name;			//pointeur vers valeur de nom.
		unsigned int lkey;			//pointeur vers chaine de pointeur de clés (0 si l'instance est une valeur ou clé finale).
		unsigned int lvalue;		//pointeur vers chaine de pointeur de valeurs (premiere valeur = valeur par défaut de la clé).
		unsigned int security;		//Id sécurité.
							//Quand index est,une valeur, security représente la clé mère.
} __attribute__ ((packed));

struct Arbre//début
{
	unsigned char dimension;//0=libre, 1=noeud, 2=valeur
	unsigned int entry;//point d'entrée
	unsigned int free;//début zone libre
}__attribute__((packed));

struct Node
{
//	unsigned char type;//0=libre, 1=noeud de branches, 2=noeud de feuilles
//	unsigned char nb;//nombre d'élément
	uchar size;//taille totale du node
	unsigned int n_prec;//element precedent
	unsigned int n_suiv;//element suivant
	unsigned int n_pere;//noeud père
	unsigned int n_fils_d;//Noeud fils gauche
	unsigned int valeur;
	uchar nom[];
}__attribute__((packed));

struct Branche
{
	unsigned char next;//branche suivante ou  0=dernière
	unsigned int N_fils;//Noeud suivant avant premier element
	unsigned char size;//taille
	unsigned char* text;//contenu
}__attribute__((packed));

struct Element2//feuille
{
	unsigned int before;//pos branche/feuille précédente
	unsigned int next;//pos branche/feuille suivant
	unsigned int value;//valeur ou branche fille
	unsigned char size;//taille
	unsigned char* text;//contenu
}__attribute__((packed));

struct Info
{
	unsigned int feuille;//pos feuille précédente
	unsigned int noeud;//pos feuille suivant
	unsigned int value;
}__attribute__((packed));
