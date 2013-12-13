/**********************************************************************
* 数据库监护人
* 处理数据库的断线重连，维持数据库连接的心心跳
* Auth: lontoken  lontoken@gmail.com
* Build: 2013.12.13
**********************************************************************/

#ifndef DBGUARDER_H
#define DBGUARDER_H

#pragma once

#define OTL_ORA11G
#define OTL_MAP_SQL_VARBINARY_TO_RAW_LONG
#define OTL_STREAWM_READ_ITERTOR_ON
#define OTL_ORA_UTF8
#include "otlv4.h"
#include <iostream>

using namespace std;

enum DbFlag{
    DF_Uninit = 0,
    DF_Inited = 1,
    DF_Except = 2,
    DF_OK = 3
};

class DbGuarder{
public:
    DbGuarder(otl_connect& db):dbConn(db){
        if(db.connected > 0){
            flag = DF_Inited;
        }else{
            flag = DF_Uninit;
        }
    };

    //对应的链接有异常
    int Except();

    //确保是否做好了准备，开始执行数据库访问
    bool MakeSurePrepared();
private:

    otl_connect& dbConn;
    DbFlag flag;
};

#endif