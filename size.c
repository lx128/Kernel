#include <stdio.h>

FILE * f1;

int main (int argc, char **argv)
{
	unsigned long comp;
	unsigned char a;

	if (argc!=2)
	{
		printf ("Error: Number of arguments is wrong\n");
		return 0;
	}

	f1=fopen(argv[1],"r");
	if (f1==0)
	{
		printf ("Error: Can't open file %s\n",argv[1]);
		return 0;
	}

	fseek(f1,0L,2);
	comp = ftell(f1);

/*	a = (comp>>24) & 0xFF;
	printf ("%c",a);
	a = (comp>>16) & 0xFF;
	printf ("%c",a);
	a = (comp>>8) & 0xFF;
	printf ("%c",a);
	a = comp & 0xFF;
	printf ("%c",a);*/

	a = comp & 0xFF;
	printf ("%c",a);
	a = (comp>>8) & 0xFF;
	printf ("%c",a);
	a = (comp>>16) & 0xFF;
	printf ("%c",a);
	a = (comp>>24) & 0xFF;
	printf ("%c",a);

	fclose (f1);
}
