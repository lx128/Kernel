extern int ValueX4(int);
extern int ValueX3(int);

const int VALUE=5;
const char message[]="Salut comment va?\n";

int ValueX4(int var)
{
	return var*VALUE;
}

int ValueX3(int var)
{
	return var*3;
}

char* ValueString(void)
{
	return message;
}
