#ifndef _SERIALGEN_H
#define _SERIALGEN_H

#include "sys/sync.h"

class SerialGenerater{
public:
    SerialGenerater():iSerial(0){};
    ~SerialGenerater(){};

public:
    int InitSerial(const int iVal){
        cri.Lock();
        iSerial = iVal;
        cri.Unlock();
        return iVal;
    };

    int Generate(){
        cri.Lock();
        int iRes = ++iSerial;
        cri.Unlock();
        return iRes;
    };
private:
    int iSerial;
    CCriticalSection cri;
};

#endif