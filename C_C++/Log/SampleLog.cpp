#include <iostream> 
#include <fstream> 
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include "SampleLog.h"
using namespace std; 

static ofstream GSampleLogFile;
static bool GFlagHasInitSampleLog = false;
static time_t GLogTimer;
static clock_t GLogClock;

void InitSampleLog()
{
    if(!GFlagHasInitSampleLog){
        GSampleLogFile.open(FILE_LOG_NAME, std::ofstream::out | std::ofstream::app);
        GLogTimer = time(NULL);
        GLogClock = clock();
        GSampleLogFile << "***************************************" << endl; 
        GFlagHasInitSampleLog = true;
    }
}

void UnInitSampleLog()
{
    if(GFlagHasInitSampleLog){
        GSampleLogFile.close();
        GFlagHasInitSampleLog = false;
    }
}

int WriteSampleLog(const char *pMsg, const char* file, int line)
{
    static struct tm *local;
    GLogTimer = time(NULL);
    clock_t curClock = clock();
    local = localtime(&GLogTimer);

    if(GFlagHasInitSampleLog){
        //日志格式:  [2013-5-9 22:39:22::15:0:0 SampleLog.cpp:64]:****
        GSampleLogFile << "[" << local->tm_year + 1900 << "-" << local->tm_mon + 1 << "-" << local->tm_mday << " " 
            << local->tm_hour << ":" << local->tm_min << ":" << local->tm_sec << "::" 
            << curClock << ":" << curClock*1.0/CLOCKS_PER_SEC << ":" << (curClock - GLogClock) << " " << file << ":" << line << "]:" << pMsg << endl;        
    }

    GLogClock = curClock;
    
    return 0;
}

int main()
{
    InitSampleLog();

    char pNum[10];
    memset(pNum, 0, sizeof(pNum));

    int idx = 0;
    while(idx < 10000){
        memset(pNum, 0, sizeof(pNum));
        sprintf(pNum, "%d", idx);
        WRITESAMPLELOG(pNum);
        ++idx;

        if(idx % 10000 == 0){
            std::cout << idx << std::endl;
        }
    }

    UnInitSampleLog();
    return 0;
}