#ifndef _HASHSEARCH_H
#define _HASHSEARCH_H

#pragma once

#include <stdio.h>  
#include <ctype.h>     //��лcityloveָ���� 

const int HashTableSize = 175447;

typedef struct
{
    unsigned long nHashA;
    unsigned long nHashB;
    char bExists;
    char p[9];
}MPQHASHTABLE;

  
//���µĺ�������һ������Ϊ0x500����10��������1280����cryptTable[0x500]  
void prepareCryptTable();

MPQHASHTABLE *  prepareHashTable();
  
//���º�������lpszFileName �ַ�����hashֵ������dwHashType Ϊhash�����ͣ�  
//������GetHashTablePos����������ñ������������ȡ��ֵΪ0��1��2���ú���  
//����lpszFileName �ַ�����hashֵ��  
unsigned long HashString( char *lpszFileName, unsigned long dwHashType );

int AddHashItem(char *lpszString, MPQHASHTABLE *lpTable, int nTableSize);

int GetHashTablePos( char *lpszString, MPQHASHTABLE *lpTable, int nTableSize, int *pos);

void printTable();

#endif