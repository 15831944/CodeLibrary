#include "UnicodeConverter.h"
#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;
int main()
{
    string inFileNmae = "in.txt";
    FILE *fIn = fopen(inFileNmae.c_str(), "rt");
    if(NULL == fIn){
        cout << "open file " << inFileNmae << " faild."  << endl;
        return 1;
    }

    fseek(fIn, 0, SEEK_END);
    int length = ftell(fIn);
    char* data = new char[length];
    rewind(fIn);
    fread((void*)data, sizeof(char), length, fIn);
    fclose(fIn);

    cout << "in: " << data << endl;

    string outFileNmae = "out.txt";

    cout << "end." << endl;
    char c;
    cin >> c;

    return 0;
}