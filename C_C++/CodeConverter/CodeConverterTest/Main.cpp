#include "CodeConverter.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void test()
{
    string finNmae = "in.txt";
    ifstream fin;
    fin.open(finNmae.c_str());

    if (!fin.is_open()){
        cout << "opening file error." << finNmae.c_str() << endl; 
        exit (1);
    }

    const int LINE_LENGTH = 1024 * 5;
    char strLine[LINE_LENGTH];

    int line = 0;
    CodeType cType;
    while(!fin.eof()){
        fin.getline(strLine, LINE_LENGTH);
        line++;

        if(line == 1){
            cType = CCodeConverter::GetCodeType((unsigned char*)strLine, strlen(strLine));
            cout << "CodeType: " << cType << endl;
        }else{

        }

        
        cout << strLine << endl;
    }
}


int main()
{
    /*string inFileNmae = "in.txt";
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

    string outFileNmae = "out.txt";*/

    test();

    cout << "end." << endl;
    char c;
    cin >> c;

    return 0;
}