/*CLR All rights Reserved*/
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int là où necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//Sécurité                                                      =0

#include "Objet.h"
#include <stdlib.h>
/*Objet de taille 8 char, 16 32 pour commencer
Créer une table pour chaque taille d'objet
Créer une autre table qui référence chaque objet, leur handle créateur
Le nom et les droits seront consultable dans la BDR
*/

//Extern
long ObjCreate(char, char, char*, char*, char);//type, size, valeurs, name, sizename
long ObjRead(long, char*);//handle, outputbuffer
long ObjWrite(long, char*);//handle, valeurs
long ObjDelete(long);//handle

/* TODO quand la BDR sera operationelle
Operation avec le nom de l'objet
long ObjNRead(char*, char, char*);//nom, size of name, outputbuffer
long ObjNWrite(char*, char, char*);//nom, size of name, valeurs
long ObjNDelete(char*, char);//nom, size of name
*/


//intern
static long* mtable=0, mtable_obj=0;//adresse des tables

// sera retiré car ne sert que pour le test, il faudra passé par alloc dans le kernel
static long table_entry(1024)={0}, table_obj_entry(1024)={0};
//static long* table_entry=0, table_obj_entry=0;

static long page_size=0;

int main(void)
{
    Obj
    return 0;
}

long ObjCreate(char type, char size, char* value, char* name, char sizename)
{
     short comp1=0, comp2=0, nb_entry=0, nb_entry2=0, t_nb=0, t_pos=0, find=0;
     struct obj_entry *t_obj;
//obj exist ? TODO recherche si le nom existe deja
/*      if (OpenObj())
      {
                    return 0;//Obj exist
      }*/
//find empty location in mtable pour entrée_obj
       comp1=0;
       comp2=0;
       find=0;
       nb_entry=page_size/4;//nombre d'entrée dans table maitre
       nb_entry2=page_size/sizeof(obj_entry);//nombre d'entrée dans table_obj
       while (comp1=! nb_entry)//tant que la fin de la table n'est pas atteinte
       {
             if(mtable[comp1]!=0)//entrée de table trouvé
             {
                            t_obj= (*struct)(table_obj[comp1]+ sizeof(long));
                            while (comp2=! nb_entry2)//tant que la fin de la table d'objet n'est pas atteinte
                            {
                                  if(t_obj->size ==0)//entrée libre trouvé
                                  {
                                      t_nb= comp1;
                                      t_pos= comp2;
                                      comp1=nb_entry;
                                      comp2=nb_entry2;
                                      find=1;
                                  }
                                  comp2++;
                            }
             }

       comp1++;
       }

//sinon, create new table pour l'entrée de l'obj
         if (find==0)
         {
         
         }

//find empty location in mtable_obj
       comp1=0;
       comp2=0;
       find=0;
       nb_entry=page_size/4;//nombre d'entrée dans table maitre
       nb_entry2=page_size/sizeof(obj_entry);//nombre d'entrée dans table_obj
       while (comp1=! nb_entry)//tant que la fin de la table n'est pas atteinte
       {
             if(mtable[comp1]!=0)//entrée de table trouvé
             {
                            t_obj= (*struct)(table_obj[comp1]+ sizeof(long));
                            while (comp2=! nb_entry2)//tant que la fin de la table d'objet n'est pas atteinte
                            {
                                  if(t_obj->size ==0)//entrée libre trouvé
                                  {
                                      t_nb= comp1;
                                      t_pos= comp2;
                                      comp1=nb_entry;
                                      comp2=nb_entry2;
                                      find=1;
                                  }
                                  comp2++;
                            }
             }

       comp1++;
       }
//sinon, create new table
         if (find==0)
         {
         
         }
//register obj
//register in BDR

}

long ObjInit()
{
     //page_size=GetPageSize();
page_size=4096;//pour les test

//Init mtable
       mtable= malloc(page_size);
       mtable_obj= malloc(page_size);
}

long FindFree()
{
     short comp1=0, comp2=0, nb_entry=0, nb_entry2=0, t_nb=0, t_pos=0;
     struct obj_entry *t_obj;

       comp1=0;
       comp2=0;
       nb_entry=page_size/4;//nombre d'entrée dans table maitre
       nb_entry2=page_size/sizeof(obj_entry);//nombre d'entrée dans table_obj
       while (comp1=! nb_entry)//tant que la fin de la table n'est pas atteinte
       {
             if(mtable[comp1]!=0)//entrée de table trouvé
             {
                            t_obj= (*struct)(table_obj[comp1]+ sizeof(long));
                            while (comp2=! nb_entry2)//tant que la fin de la table d'objet n'est pas atteinte
                            {
                                  if(t_obj->size ==0)//entrée libre trouvé
                                  {
                                      t_nb= comp1;
                                      t_pos= comp2;
                                      comp1=nb_entry;
                                      comp2=nb_entry2;
                                      find=1;
                                  }
                                  comp2++;
                            }
             }

       comp1++;
       }
}
