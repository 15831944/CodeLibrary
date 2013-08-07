#include "HashSearch.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

//crytTable[]里面保存的是HashString函数里面将会用到的一些数据，在prepareCryptTable  
//函数里面初始化  
unsigned long cryptTable[0x500];  
MPQHASHTABLE * pHashTable = NULL;

//以下的函数生成一个长度为0x500（合10进制数：1280）的cryptTable[0x500]  
void prepareCryptTable()  
{   
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;  

    for( index1 = 0; index1 < 0x100; index1++ )  
    {   
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )  
        {   
            unsigned long temp1, temp2;  

            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp1 = (seed & 0xFFFF) << 0x10;  

            seed = (seed * 125 + 3) % 0x2AAAAB;  
            temp2 = (seed & 0xFFFF);  

            cryptTable[index2] = ( temp1 | temp2 );   
        }   
    }   
}  

MPQHASHTABLE * prepareHashTable()
{
    pHashTable = (MPQHASHTABLE *)malloc(HashTableSize * sizeof(MPQHASHTABLE));
    memset(pHashTable, 0, HashTableSize * sizeof(MPQHASHTABLE));
    return pHashTable;
}

//以下函数计算lpszFileName 字符串的hash值，其中dwHashType 为hash的类型，  
//在下面GetHashTablePos函数里面调用本函数，其可以取的值为0、1、2；该函数  
//返回lpszFileName 字符串的hash值；  
unsigned long HashString( char *lpszFileName, unsigned long dwHashType )  
{   
    unsigned char *key  = (unsigned char *)lpszFileName;  
    unsigned long seed1 = 0x7FED7FED;  
    unsigned long seed2 = 0xEEEEEEEE;  
    int ch;  

    while( *key != 0 )  
    {   
        ch = toupper(*key++);  

        seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);  
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;   
    }  
    return seed1;   
}  

int AddHashItem(char *lpszString, MPQHASHTABLE *lpTable, int nTableSize)
{
    const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;

    unsigned long  nHash = HashString( lpszString, HASH_OFFSET );
    unsigned long nHashA = HashString( lpszString, HASH_A );
    unsigned long nHashB = HashString( lpszString, HASH_B );
    int  nHashStart = nHash % nTableSize;
    int  nHashPos = nHashStart;

    //std::cout << lpszString << ":" << nHash << ":"  << nHashA << ":" << nHashB << ":"  << nHashPos << ":"  << std::endl;

    
    while(lpTable[nHashPos].bExists)
    {
        nHashPos = (nHashPos + 1) % nTableSize;

        if (nHashPos == nHashStart)
            return -1;
    }

    lpTable[nHashPos].bExists = true;
    lpTable[nHashPos].nHashA = nHashA;
    lpTable[nHashPos].nHashB = nHashB;
    memcpy(lpTable[nHashPos].p, lpszString, 9);

    return nHashPos;
}

int GetHashTablePos( char *lpszString, MPQHASHTABLE *lpTable, int nTableSize, int *pos)
{
    const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;

    unsigned long  nHash = HashString( lpszString, HASH_OFFSET );
    unsigned long nHashA = HashString( lpszString, HASH_A );
    unsigned long nHashB = HashString( lpszString, HASH_B );
    int  nHashStart = nHash % nTableSize;
    int  nHashPos = nHashStart;

    //std::cout << lpszString << ":" << nHash << ":"  << nHashA << ":" << nHashB << ":"  << nHashPos << ":"  << std::endl;

    
    while ( lpTable[nHashPos].bExists)
    {
        /*如果仅仅是判断在该表中时候存在这个字符串，就比较这两个hash值就可以了，不用对
        *结构体中的字符串进行比较。这样会加快运行的速度？减少hash表占用的空间？这种
        *方法一般应用在什么场合？*/
        if(lpTable[nHashPos].nHashA == nHashA && lpTable[nHashPos].nHashB == nHashB)
        {
            return nHashPos;
        }
        else
        {
            nHashPos = (nHashPos + 1) % nTableSize;
        }

        if (nHashPos == nHashStart)
            break;
    }

    //std::cout << "nHashPos:" << nHashPos << std::endl;

    *pos = nHashPos;
    return -1;
}

void printTable()
{
    int i = 0;
    /*打印数组crytTable[0x500]里面的值*/  
    for ( ; i < 0x500; i++ )  
    {  
        if ( i % 10 == 0 )  
        {  
            printf("/n");  
        }  

        printf("%-12X", cryptTable[i] );  
    }  
}