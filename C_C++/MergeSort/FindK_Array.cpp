#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void merge_sort(int* arr, int beg, int end);
void merge(int* arr, int beg, int mid, int end);

int main(){
  int len;
  cin >> len;
  int* iArr = new int[len]();
  for(int idx = 0; idx != len; ++idx){
    cin >> iArr[idx];
  }

  merge_sort(iArr, 0, len - 1);

  int k;
  cin >> k;
  cout << iArr[k - 1]; 
}

void merge_sort(int* arr, int beg, int end){
  if(end > beg){
    int mid = beg + (end - beg)/2;
    merge_sort(arr, beg, mid);
    merge_sort(arr, mid + 1, end);
    merge(arr, beg, mid, end);
  }
}

void merge(int* arr, int beg, int mid, int end){
  int iLf = mid - beg + 1;
  int * iTmp = new int[iLf]();

  for(int idx = beg, ii = 0; ii < iLf; ++idx, ++ii){
    *(iTmp + ii) = *(arr + idx);
  }

  int iF = 0;
  int iE = mid + 1;
  for(int idx = beg; idx <= end; ++idx){
    if(iF == iLf){
      *(arr + idx) = *(arr + iE++); 
    }else if(iE == end + 1){
      *(arr + idx) = *(iTmp + iF++);
    }else{
      if(*(iTmp + iF) <= *(arr + iE)){
        *(arr + idx) = *(iTmp + iF++);
      }else{
        *(arr + idx) = *(arr + iE++);
      }
    }
  }
}