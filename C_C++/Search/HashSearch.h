#ifndef _HASHSEARCH_H
#define _HASHSEARCH_H

#pragma once

#include <stdio.h>  
#include <ctype.h>     //��лcityloveָ����  

  
//���µĺ�������һ������Ϊ0x500����10��������1280����cryptTable[0x500]  
void prepareCryptTable();
  
//���º�������lpszFileName �ַ�����hashֵ������dwHashType Ϊhash�����ͣ�  
//������GetHashTablePos����������ñ������������ȡ��ֵΪ0��1��2���ú���  
//����lpszFileName �ַ�����hashֵ��  
unsigned long HashString( char *lpszFileName, unsigned long dwHashType );

void printTable();

#endif