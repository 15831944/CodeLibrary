#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <cstring>

#include "BinarySearch.h"
#include "HashSearch.h"

using namespace std;

int tableItemCount = 0;

void generateTestData(int *arr, int len)
{
    for(int i = 0; i < len; ++i){
        *(arr + i) = i;
    }
}

void BinarySearchTest(int *arr, int len)
{
    int l = len -1;
    int *pos = NULL;
    for(int i = 0; i < len; ++i){
        BinarySearch_I(arr, 0, l, i, pos);
    }
}

void generateHashSearchData_Str(MPQHASHTABLE *pHashTable)
{ 
    int i = 0;
    int idx = 0;
    int pos = 0;
    char t[9] = {0};

    for(i = 300000; i < 305000; ++i){
        sprintf(t, "%dSZ", i);
        idx = AddHashItem(t, pHashTable, HashTableSize);
        //cout << pos << " " << t << endl;
        tableItemCount++;
    }

    for(i = 600000; i < 605000; ++i){
        sprintf(t, "%dSS", i);
        idx = AddHashItem(t, pHashTable, HashTableSize);

        //cout << pos << " " << t << endl;
        tableItemCount++;
    }
}

void HashSearchTest_Str(MPQHASHTABLE *pHashTable)
{
    int i = 0;
    int idx = 0;
    char t[9] = {0};
    int pos = 0;

    for(i = 300000; i < 305000; ++i){
        sprintf(t, "%dSZ", i);
        idx = GetHashTablePos(t, pHashTable, HashTableSize, &pos);
        //memcpy(pHashTable[idx].p, t, sizeof(t));
       // cout << pHashTable[idx].p << "  " << i << endl;
        if(idx > 0){
           tableItemCount--;
        }
    }

    for(i = 600000; i < 605000; ++i){
        sprintf(t, "%dSS", i);
        idx = GetHashTablePos(t, pHashTable, HashTableSize, &pos);
        //memcpy(pHashTable[idx].p, t, sizeof(t));
       // cout << pHashTable[idx].p << "  " << i << endl;
        if(idx > 0){
           tableItemCount--;
        }
    }
}


int main()
{
    int len = 1000000;
    int *arr = (int *)malloc(len * sizeof(int));
    generateTestData(arr, len);

    LARGE_INTEGER nFreq;
    LARGE_INTEGER nBeginTime;
    LARGE_INTEGER nEndTime;
    double time;

    QueryPerformanceFrequency(&nFreq);

    QueryPerformanceCounter(&nBeginTime); 
    BinarySearchTest(arr, len);
    QueryPerformanceCounter(&nEndTime);
    time=(double)(nEndTime.QuadPart - nBeginTime.QuadPart)/(double)nFreq.QuadPart;

    cout << "BinarySearch: cost= " << time << ", per= " << (time * 1000000/ len) << ", len=" << len << endl;

    cout << "nFreq: " << nFreq.QuadPart << endl;

    MPQHASHTABLE *pHashTable = prepareHashTable();
    prepareCryptTable();  
    generateHashSearchData_Str(pHashTable);

    SYSTEMTIME sys; 
    GetLocalTime( &sys ); 
    
    printf( "%02d.%03d   %d\n", sys.wSecond, sys.wMilliseconds, tableItemCount); 

    QueryPerformanceCounter(&nBeginTime); 
    HashSearchTest_Str(pHashTable);
    QueryPerformanceCounter(&nEndTime);

    GetLocalTime( &sys ); 
    printf( "%02d.%03d    %d\n", sys.wSecond, sys.wMilliseconds, tableItemCount); 


    time=(double)(nEndTime.QuadPart - nBeginTime.QuadPart)/(double)nFreq.QuadPart;

    cout << "HashSearch: cost= " << time << ", per= " << (time * 1000000/ 10000) << endl;

    int i;
    cin >> i;
    return 0;
}