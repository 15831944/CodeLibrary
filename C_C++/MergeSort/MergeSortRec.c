#include <stdio.h> 

void Merge(int L, int R, int Mid, int A[], int Tmp[])
{
    int i = 0, TmpIndex = L, LIndex = L, RIndex = Mid+1;

    while(LIndex <= Mid){
        Tmp[i++] = A[LIndex++];
    }

    int iT = 0, iLt = i, iA = Mid + 1;
    for(i = L; i <= R; ++i){
        if(iT == iLt){
            A[i] = A[iA++];
        }else if(iA == R + 1){
            A[i] = Tmp[iT++];
        }else{
            if(Tmp[iT] <= A[iA]){
                A[i] = Tmp[iT++];
            }else{
                A[i] = A[iA++];
            }
        }
    }

    /*
    int i, TmpIndex = L, LIndex = L, RIndex = Mid+1;
     
    while(LIndex <= Mid && RIndex <= R)
    Tmp[TmpIndex ++] = A[LIndex] <= A[RIndex] ? A[LIndex ++] : A[RIndex ++];
     
    while(LIndex <= Mid)
        Tmp[TmpIndex ++] = A[LIndex ++];
    while(RIndex <= R)
      Tmp[TmpIndex ++] = A[RIndex ++];

    for(i=L; i<=R; i++)
      A[i] = Tmp[i];
    */
}
 
void MergeSort(int A[], int N)
{
    int *Tmp = (int *)malloc( (N + 1)/2 * sizeof(int) );
    int i, j, R, Mid;
    for(i=2; i<N; i <<= 1)
    {
        for(j=0; j<=N-1; j+=i)
        {
            Mid = j + (i>>1) - 1;
            if(j+i-1 > N-1)
                R = N-1;
            else
                R = j+i-1;
            Merge(j, R, Mid, A, Tmp);
        }
    }
    Merge(0, N-1, (i>>1)-1, A, Tmp);
    free(Tmp);
}

int main(){
    int len = 0;
    scanf("%d\n", &len);
    int* arr = (int *)malloc( len * sizeof(int) );
    int idx;

    
    for(idx = 0; idx < len; ++idx){
        scanf("%d", arr + idx);
    }
    
    MergeSort(arr, len);

    scanf("%d", &idx);
    printf("%d", *(arr + idx -1));

    free(arr);
}