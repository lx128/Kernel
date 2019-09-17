/*CLR All rights Reserved*/
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int là où necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//Sécurité                                                      =0

#include<stdio.h>
#include<string.h>
#include"bdr.h"
/*Valeur limite:
nom de clé 255 char
nom de valeur 16384char unicode
valeur pas de limite
arbre 512 niveaux de profondeur*/
//Visible de l'exterieur
unsigned int BDRLoad(void);                    //Charge la BDR en memoire.
//int BDRSave(void);                    //Sauve la BDR dans un fichier.
/*int BDRDefrag(void);                //Reorganise la BDR dans un autre espace.
int BDRSize(void);                    //Taille de la BDR en octet.
*/
unsigned int BDRCreateKey(unsigned char*, unsigned short);//Créé une clé.
/*int BDRCreateValue(char*, int, char, void*, int);//Créé une valeur.
int BDRRenameKey(char*, int, char, void*, int);//Renomme une clé.
int BDRRenameValue(char*, int, char, void*, int);//Renomme une valeur.
int BDRWriteValue(char*, int, char, void*, int);//Ecrit une valeur.
                                      //Name, type, value
int BDRReadValue(char*);              //Lis la valeur.
int BDREraseKey(char*);               //Efface une clé.
int BDREraseValue(char*);             //Efface une valeur.
int BDREnum(char*);                   //Liste les valeurs et les sous-clés d'une clé .
*/
//Invisible de l'exterieur
unsigned int FindFreeIndex(void);
void DumpIndex(void);
//int RetPos(char*);                    //Retourne la position de la clé/valeur
                                      //0=Non trouvé


static unsigned int FindKey(unsigned int, unsigned char *, unsigned short); //Recherche si une sous clé existe déjà dans la clé


static struct index *regi=0;	//base principale key
static unsigned char *regd=0;	//base principale value
static unsigned int ilimit =0;		//limite absolu zone key
static unsigned int dlimit =0;		//limite absolu zone value

int main()
{
    if (BDRLoad()==0){exit(1);};
	//BDRLoad();
//Clé racine
    BDRCreateKey("HKEY_LOCAL_MACHINE\\Startup\\test\\",32);
//    BDRCreateValue("File",4,"hal.dll",7,4);
/*Version du registre*/
/*Nom*/
/*Table de la racine*/
//    fwrite("sdfghjklm",1,9,regf);
//    fwrite("Salut!",1,6,regf);
//    printf("%d\n", regf);

//	BDRSave();

//	getchar();
    return;
}

//OK
unsigned int BDRLoad(void)
{
	FILE * regfile;
    unsigned char buff[HEADER_SIZE];
	struct header * this_header = (struct header*) buff;

	regi = malloc(LIMIT);
	regd = malloc(LIMIT);

    regfile = fopen("bdr.bin","rb+");//ouverture
//Lire signature
    fread (buff,HEADER_SIZE,1,regfile);

    if (strncmp(this_header->version, VERSION,8))
    {
		printf("Version invalide\n");
		return 0;//Version bdr invalide
    }

//copie de la bdr
	ilimit=this_header->key_section_size/sizeof(struct index);
	dlimit=this_header->value_section_size;
	fread (regi,this_header->key_section_size,1,regfile);
	fread (regd,dlimit,1,regfile);

	return 1;//1=ok 0=err*/
}

/*int BDRSave(void)
{
	FILE * regf;
    char * buff[HEADER_SIZE];
	struct header * this_header = (struct header) buff;
    fseek (regf,0,SEEK_SET);//remise au debut du fichier

//ecrire signature
    fwrite(reg,12,1,VERSION);
//laisse 4 octets pour la taille de la bdr
    fseek (regf,0,16);//commence apres la signature

//Reorganise bdr
    BDRDefrag();               //Reorganise la BDR dans un autre espace.

//ecriture bdr
    fwrite(reg,size_bdr,1,regf);

//ecrire de 2 x 0x0000 pour éviter qu'une lecture de chaine mal faite se termine hors memoire
    fwrite(reg,4,1, "/0/0/0/0");

//ecrire de la taille
    fseek (regf,0,12);//commence apres la signature
    fwrite(reg,4,1,(char*)&size_bdr);

    close (regf);//fermeture fichier

    return 1;//1=ok 0=err
}

/*int BDRWKey(char* name, int size, char type, void* value, int size2);      //ecrit ou créé une clé/valeur.
{
    static char* str1[32]={0};
    static char count=0;
    static int count2=0, pos=0, lastpos=0;

    count2=0;
    switch (type)
    {
    case 0: //key
    case 1: //dword
         while(count2<size)
         {
         while (name[count2]!="\\" | count2<size | count<32)        //à verifier
         {
               name[count2]=str1[count];
               count2++;
               count++;
         }

         if (count==32)//clé
         {
            return 0;//error nom trop long
         }

         if (name[count2]=="\\")//clé
         {
                  lastpos=pos;
                  pos = RetPos(pos,str1,(count-1));
                  if (pos ==0)//pas de clé trouvé donc création
                  {
                          //
                  }
         }
         else //valeur
         {
                  if (RetPos());
         }
         count2++;//saute le \
         count=0;
         }
    break;
    }
}

int RetKeyPos(long offset, char* name, int size);                    //Retourne la position de la clé
{


    return 0;//pas trouvé
    return pos;
}
*/
//OK
unsigned int BDRCreateKey(unsigned char* chemin, unsigned short size)//Créé une clé.
{
	static unsigned char name[256];
	static unsigned short comp=0, comp2=0;
	static unsigned int parent=0,current=0;

	comp=0;
	comp2=0;
	parent = 0;
//	printf("%s\n",chemin);
	for (comp=0;comp<size;comp++)//exploration du chemin et ajout des clés manquantes.
	{
		if (chemin[comp]==0x5C)
		{
			name[comp2]=0;
			printf("%s\n",name);

			current = FindKey(parent,name,comp2);//recherche si la clé existe

			if (current == 0)//La clé n'existe pas, on la créé
			{
				CreateKey(name,parent);
			}
			comp2=0;
		}
		else
		{
			name[comp2]=chemin[comp];
			comp2++;
		}
	}
	return 0;
}

int CreateKey(char * name, unsigned int parent)
{
	static unsigned int comp = 0, id_libre=0;
	static unsigned short* old_list_size=0, *new_list_size=0;
	static unsigned int* old_key_list=0, *new_key_list=0;

//	CheckDefrag();//teste si defrag necessaire

	comp = FindFreeIndex();
	if (comp==0)//si retourne 0 alors rien de libre
	{
		printf("BDR: Error, No more free entry\n");//pas d'entry libre
		return 0;
	}
	DumpIndex();
	printf("%d\n",comp);

	//récupération chaine de clé du parent.
	old_list_size= (unsigned short*)(regd + regi[parent].lkey);
	new_list_size = (unsigned short*)(regd + dlimit);
	old_key_list = (unsigned int*)(old_list_size+1);
	new_key_list = (unsigned int*)(new_list_size+1);
printf("%d %d %d %d\n", old_list_size, new_list_size,old_key_list, new_key_list);
	//copie de la nouvelle chaine
	strncpy(new_key_list, old_key_list, *old_list_size);

	//ajout de la clé et changement de la taille
//	new_key_list[*old_list_size/2]=comp;
printf("%d\n",(*old_list_size/2));
//	new_list_size = new_list_size +4;

	//création de la nouvelle clé dans index
	regi[comp].type = 0x1;//type clé
	//regi[comp].
	return 0;
}
/*
int AddSKey(int source, int lim) //Ajoute une SubKey à une Key
{
	static struct key* k = 0;
	static struct v_key* v = 0, v2 = 0;
	static unsigned i = 0;
	i=0;
	k = lim + &reg;
	v = k->Skey + &reg;//pointe vers les valeurs de la key
	v2 = valueend + &reg;
	CheckDefrag();//teste si defrag necessaire
	while (v->value[i]!=0) //copie ancienne keys vers nouvelles place
	{
		v2->value[i] = v->value[i];
		valueend=+4;
		i++;
	}
	v2->value[i] = lim;
		valueend=+4;
	v2->value[i] = v->value[i];
		valueend=+4;
}
int DelSKey(int source, int lim) //Efface une SubKey à une Key
{
	static struct key* k = 0;
	static struct v_key* v = 0, v2 = 0;
	static unsigned i = 0;
	i=0;
	k = lim + &reg;
	v = k->Skey + &reg;//pointe vers les valeurs de la key
	v2 = valueend + &reg;
	CheckDefrag();//teste si defrag necessaire
	while (v->value[i]!=0) //copie ancienne keys vers nouvelles place
	{
		v2->value[i] = v->value[i];
		valueend=+4;
		i++;
	}
	v2->value[i] = lim;
		valueend=+4;
	v2->value[i] = v->value[i];
		valueend=+4;
}
*/
//OK
unsigned int FindFreeIndex (void) //Recherche un index libre
{
	static unsigned int comp = 0, limite=0;

	limite=ilimit/sizeof(index)+1;
//printf("Limite index: %d\n", limite);
	for (comp=0;comp < limite;comp++)//Recherche d'un entry libre
	{
//printf("Index testé: %d type: %d ", comp,regi[comp].type);
//printf("%d\n",regi[comp].name);
		if (regi[comp].type == 0)//entry libre trouvée
		{
//printf("Libre %d\n",comp);
			break;
		}
	}

	if (comp==limite)//si retourne la derniere clé alors rien de libre
	{
		return 0;
	}
	return comp;
}

//OK
unsigned int FindKey(unsigned int source, unsigned char *nom, unsigned short size) //Recherche si une sous clé existe déjà dans la clé
{
	static unsigned int this_key=0, this_name=0, *string_of_key=0, comp=0;
	static unsigned char * name=0;
	static unsigned short *this_size=0, *this_name_size=0;
	static int find = 0;//résultat

	if (regi[source].type!=1)
	{
printf("11 %d\n",regi[source].type);
		return 0;//la source n'est pas une clé valide
	}
	this_size = (short*)(regi[source].lkey+regd);//taille de la liste des sous clés
	string_of_key = (int*)(regi[source].lkey+regd+2);//liste des sous clés offset
printf("regi lkey: %d\n",regi[source].lkey);
printf("regd: %d :%s\n",regd,regd);
printf("this size%d\n", *this_size);
	for (comp=0;comp<*this_size;comp++)//teste toute les sous clés
	{
		this_key = string_of_key[comp];//teste cette clé offset
printf("this key%x\n",this_key);
printf("22\n");
		if (this_key>=LIMIT)//teste si valeur hors limite
		{
printf("Limit\n");
			return 0;//valeur hors limite
		}
		this_name = regi[this_key].name+2;//nom de cette clé offset
		this_name_size = (short*)(regd + regi[this_key].name);//taille du nom de cette clé
printf("33 name size%d name pos %x\n",*this_name_size, this_name);
		if ((*this_size+ *this_name_size)>=LIMIT)//teste si valeur hors limite
		{
printf("Valeur This_name hors Limite\n");
			return 0;//valeur hors limite
		}
		if (*this_name_size>=512)//teste si valeur hors limite
		{
printf("Valeur hors Limite\n");
			return 0;//valeur hors limite
		}

printf("44 taille source %d\n",size);
		if (size == *this_name_size)//recherche par la taille
		{
			name = (char *)regd + this_name;
printf("55 %s\n",name);
			if (strncmp(name, nom, size)==0)//si taille bonne, comparaison des chaines
			{//trouvé
			printf("Trouvé!!!!!!! %d\n", this_key);
				return this_key;
			}
		}
	}
printf("Fin\n");
	return 0;
}
/*
int CheckDefrag(void); //teste si defrage est necessaire. En général si il reste moins de 4096 octets de libre en zone Value
{
	return 1;//TO DO
}
*/

void DumpIndex(void) //teste si defrage est necessaire. En général si il reste moins de 4096 octets de libre en zone Value
{
	static unsigned int comp=0;
	static unsigned char *ptr;

	ptr = (unsigned char*)regi;
	printf("limite: %d\n",(ilimit*sizeof(struct index)));
	for(comp=0;comp<(ilimit*sizeof(struct index));comp++)
	{
		printf("%X ",ptr[comp]);
	}
}
