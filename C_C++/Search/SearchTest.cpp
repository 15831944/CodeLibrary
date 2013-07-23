#include <iostream>
#include <cstdlib>
#include <windows.h>

#include "BinarySearch.h"

using namespace std;

void generateTestData(int *arr, int len)
{
    for(int i = 0; i < len; ++i){
        *(arr + i) = i;
    }
}

void BinarySearchTest(int *arr, int len)
{
    int l = len -1;
    int *pos;
    for(int i = 0; i < len; ++i){
        BinarySearch_I(arr, 0, l, i, pos);
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

    cout << "BinarySearch: cost= " << time << ", per= " << (time * 1000000/ len) << endl;

    cout << "nFreq: " << nFreq.QuadPart << endl;

    int i;
    cin >> i;
    return 0;
}