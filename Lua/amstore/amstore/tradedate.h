#ifndef _AM_TRADEDATA_H
#define _AM_TRADEDATA_H

#include <list>
#include "typedef.h"
#include "sys/sync.h"

//�������ݻ���,һ�����ʻ��Ľ�������
//���������У�
//  ��ӡ���ȡ...
//  ����Ļع�
//  �������ݱ��棬ͨ���ַ������棬��������version_id�����±�־
//  �̰߳�ȫ
class TradeData{
public:
    TradeData(){}
    virtual ~TradeData(){}
    virtual int Init() = 0;
public:
    virtual int GetDbUpdateInfo(std::list<DBWriteItem*>& pList, int *insert, int *update) = 0;
    virtual int DoneDbUpdata(std::list<DBWriteItem*>& pList) = 0;

    virtual int InitSaveInfo() = 0;
    virtual int GetSaveInfo(std::list<DBWriteItem*>& pList) = 0;
    virtual int DoneSaveInfo() = 0;

    virtual bool Lock(int mill = 0) = 0;
    virtual void Unlock() = 0;
};

#endif