#include "HashSearch.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

//crytTable[]���汣�����HashString�������潫���õ���һЩ���ݣ���prepareCryptTable  
//���������ʼ��  
unsigned long cryptTable[0x500];  
MPQHASHTABLE * pHashTable = NULL;

//���µĺ�������һ������Ϊ0x500����10��������1280����cryptTable[0x500]  
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

//���º�������lpszFileName �ַ�����hashֵ������dwHashType Ϊhash�����ͣ�  
//������GetHashTablePos����������ñ������������ȡ��ֵΪ0��1��2���ú���  
//����lpszFileName �ַ�����hashֵ��  
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
        /*����������ж��ڸñ���ʱ���������ַ������ͱȽ�������hashֵ�Ϳ����ˣ����ö�
        *�ṹ���е��ַ������бȽϡ�������ӿ����е��ٶȣ�����hash��ռ�õĿռ䣿����
        *����һ��Ӧ����ʲô���ϣ�*/
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
    /*��ӡ����crytTable[0x500]�����ֵ*/  
    for ( ; i < 0x500; i++ )  
    {  
        if ( i % 10 == 0 )  
        {  
            printf("/n");  
        }  

        printf("%-12X", cryptTable[i] );  
    }  
}