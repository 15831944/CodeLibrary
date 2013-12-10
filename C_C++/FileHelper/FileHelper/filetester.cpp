#include "filetester.h"
#include <Windows.h>
#include <iostream>
#include <tchar.h>

using namespace std;

void filets::TestFindFirstFile(TCHAR* pPath)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    hFind = FindFirstFile(pPath, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE){
        printf("FindFirstFile failed (%d)\n", GetLastError());
        return;
    }else{
        _tprintf(TEXT("The first file found is: %s\n"), 
            FindFileData.cFileName);
        FindClose(hFind);
    }
}