#ifndef _HASHSEARCH_H
#define _HASHSEARCH_H

#pragma once

#include <stdio.h>  
#include <ctype.h>     //多谢citylove指正。  

  
//以下的函数生成一个长度为0x500（合10进制数：1280）的cryptTable[0x500]  
void prepareCryptTable();
  
//以下函数计算lpszFileName 字符串的hash值，其中dwHashType 为hash的类型，  
//在下面GetHashTablePos函数里面调用本函数，其可以取的值为0、1、2；该函数  
//返回lpszFileName 字符串的hash值；  
unsigned long HashString( char *lpszFileName, unsigned long dwHashType );

void printTable();

#endif