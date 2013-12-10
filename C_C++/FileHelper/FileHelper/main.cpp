#include <windows.h>
#include <iostream>
#include "filetester.h"
#include "filehelper.h"

using namespace std;


int main(int argc, char* argv[])
{
    filets::TestFindFirstFile(L"D:\\work");
    filets::TestFindFirstFile(L"D:\\workno");

    BOOL isE = filehp::FilePathExists(L"D:\\work");
    cout<< "work:" << isE<<endl;
    isE = filehp::FilePathExists(L"D:\\workno");
    cout<< "workno:" << isE<<endl;
    isE = filehp::FilePathExists(L"d:\\Work\\IFProj\\sbhbstore\\sbhbstore.suo");
    cout<< "sbhbstore.suo:" << isE<<endl;

    char c;
    cin >> c;
    return 0;
}

