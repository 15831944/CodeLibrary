#include <stdio.h>  
#include <ctype.h>     //��лcityloveָ����  
//crytTable[]���汣�����HashString�������潫���õ���һЩ���ݣ���prepareCryptTable  
//���������ʼ��  
unsigned long cryptTable[0x500];  
  
//���µĺ�������һ������Ϊ0x500����10��������1280����cryptTable[0x500]  
void prepareCryptTable();
  
//���º�������lpszFileName �ַ�����hashֵ������dwHashType Ϊhash�����ͣ�  
//������GetHashTablePos����������ñ������������ȡ��ֵΪ0��1��2���ú���  
//����lpszFileName �ַ�����hashֵ��  
unsigned long HashString( char *lpszFileName, unsigned long dwHashType );