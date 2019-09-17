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

struct header
{
		char version[8];
		int key_section_size;
		int value_section_size;
} __attribute__ ((packed));

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

struct data
{
		char* value;	//nom de la valeur ou de la clé
						//liste d'offset de valeur ou de clé dans index
						//Dans ces 2 cas, commence par 2 octets représentant la taille
						//peut être aussi une chaine, la longueur est dans l'index
} __attribute__ ((packed));
