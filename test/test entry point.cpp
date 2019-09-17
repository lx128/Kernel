/*Test de compilation c++ pour kernel

Compilation:
g++ test.cpp -c -s -nostdlib -o test.o
ld test.o -e _Z3runv -o test.exe

nm obj.o pour avoir le nom des symboles et du point d'entrée.
*/
class obj
{
public: 	obj()
	{
		return;
	}
};

void run()
{
//	obj *o=new obj;
}
