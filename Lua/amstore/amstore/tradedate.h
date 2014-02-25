#ifndef _AM_TRADEDATA_H
#define _AM_TRADEDATA_H

//交易数据基类,一个子帐户的交易数据
//基本功能有：
//  添加、获取...
//  事务的回滚
//  交易数据保存，通过字符串保存，保存后根据version_id来更新标志
//  线程安全
class TradeData{
public:
    TradeData(){}
    virtual ~TradeData(){}
};

#endif