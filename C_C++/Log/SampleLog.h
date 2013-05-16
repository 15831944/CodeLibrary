#ifndef _SAMPLE_LOG_H
#define _SAMPLE_LOG_H

#define FILE_LOG_NAME "SampleLog.txt"

#define WRITESAMPLELOG(msg) WriteSampleLog((msg), __FILE__, __LINE__)

void InitSampleLog();

void UnInitSampleLog();

int WriteSampleLog(const char *pMsg, const char* file = "", int line = 0);


#endif