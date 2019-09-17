#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4194304
FILE * f1;

//emplacement du fichier
int main(int argc, char **argv)
{
	unsigned int comp;
	unsigned char* buff;

	if (argc<2)
	{
		printf ("Erreur: Nom de fichier manquant\n");
		return 1;
	}

	f1=fopen(argv[1],"a+");
	if (f1==0)
	{
		printf ("Erreur: Impossible d'ouvrir %s\n",argv[1]);
		return 1;
	}

	//replace en fin de fichier
	buff=malloc(SIZE);
	printf("Depart\n");
	srand(time(NULL));

	for(comp=0;comp<SIZE;comp++)
	{
		buff[comp]=rand()%256;
	}

	while(fwrite(buff,1,SIZE,f1));

	printf("Fin. Le disque est plein\n");
	fclose(f1);
	return 0;
}
