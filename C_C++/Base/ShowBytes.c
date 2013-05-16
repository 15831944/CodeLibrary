/**********************************************************************
*  显式 int\float\指针 的机器编码 
*  Auth: lontoken 
*  Build: 2012.05.24
**********************************************************************/
#include <stdio.h>

typedef unsigned char * byte_pointer;

void show_bytes(byte_pointer start, int len)
{
	int i;
	for(i = 0; i < len; ++i){
		printf("%.2x", start[i]);
	}
	printf("\n");
}

void show_int(int x)
{
	show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x)
{
	show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x)
{
	show_bytes((byte_pointer)&x, sizeof(void *));
}

int main()
{
	int iVal = 3510593;
	float fVal = (float)iVal;
	int *pVal = &iVal;
	show_int(iVal);
	show_float(fVal);
	show_pointer(pVal);
}