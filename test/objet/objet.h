/*CLR All rights Reserved*/
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int l� o� necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//S�curit�                                                      =0

struct obj_entry//128b
{
       int handle; //handle cr�ateur
       short table; //numero de table
       short pos;   //pos dans la table
       char type;   //Type 0=inconnu
       char size;   //Size 0=effacer 8=8char 16=16 32=32
       short reserved;
       int reserved2;
} __attribute__ ((packed));

struct table_entry//64b
{
       char size;        //Size 0=effacer 8=8char 16=16 32=32
       char complete:1;  //Table Pleine=1
       char reserved2:7;
       long pos;         //Pos de la table
       short reserved;
} __attribute__ ((packed));

/*Les champs de taille variable comme le nom ou les droits d'utilisateur
seront dans la BDR*/

//Au debut de chaque table, un champs de bits r�f�rence les places libres.
/*
MTable Obj entry              Ressence les tables des entr�e d'objet
      |
      ->Table Obj entry       Ressence les entr�es obj
              |
              ->Table Nb      et contient l'entr�e de la table qui contient l'obj


MTable Entry                  Ressence les tables d'entr�e des tables
       |
       ->Table Entry          Ressence les entr�es des tables
               |
               ->Table obj    qui contiennent l'obj
                       |
                       ->OBJ
*/

/*
Erreur:
       1=meme nom
       2=memoire pleine
       3=objet pas trouv�
*/
