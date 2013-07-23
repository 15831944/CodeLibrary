#include "BinarySearch.h"

/**
* @return >0 找到了数据  -1 没有找到
**/
int BinarySearch_I(int *arr, int low, int high, int target, int *pos)
{
    int mid = 0;
    while(!(high < low))
    {
        mid = (low + high) / 2;
        if (target < arr[mid])
            high = mid - 1;
        else if (arr[mid] < target)
            low = mid + 1;
        else //find the target
            return mid;
    }

    *pos = mid;
    return -1;
}