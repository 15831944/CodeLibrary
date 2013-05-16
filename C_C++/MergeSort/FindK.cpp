#include <iostream>
#include <vector>
#include <iterator>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void merge_sort(vector<int>::iterator beg, vector<int>::iterator end);
void merge(vector<int>::iterator beg, vector<int>::iterator mid, vector<int>::iterator end);

int main(){
  int len;
  cin >> len;
  vector<int> iVec(len);
  for(vector<int>::iterator iter = iVec.begin(); iVec.end() != iter; ++iter){
    cin >> *iter;
  }

  merge_sort(iVec.begin(), iVec.end());

  int k;
  cin >> k;
  cout << iVec[k - 1]; 
}

void merge_sort(vector<int>::iterator beg, vector<int>::iterator end){
  if(end - beg > 1){
    vector<int>::iterator mid = beg + (end - beg)/2; 
    merge_sort(beg, mid);
    merge_sort(mid, end);
    merge(beg, mid, end);
  }
}

void merge(vector<int>::iterator beg, vector<int>::iterator mid, vector<int>::iterator end){
  vector<int> front(beg, mid);
  vector<int> back(mid, end);

  vector<int>::iterator iFnt = front.begin();
  vector<int>::iterator iBak = back.begin();

  for(vector<int>::iterator iter = beg; iter != end; ++iter){
    if(iFnt == front.end()){
      *iter = *(iBak++);
    }else if(iBak == back.end()){
      *iter = *(iFnt++);
    }else{
      if(*iFnt <= *iBak){
        *iter = *(iFnt++);
      }else{
        *iter = *(iBak++);
      }
    }
  }
}