#define OTL_ORA11G
#define OTL_MAP_SQL_VARBINARY_TO_RAW_LONG
#define OTL_STREAWM_READ_ITERTOR_ON
#define OTL_ORA_UTF8

#include <windows.h>
#include "otlv4.h"  
#include <iostream>
using namespace std;
#include "dbguarder.h"
#include <stdio.h>


otl_connect db; // connect object

void insert()
{ 
    otl_stream o(50,  "insert into test_tab values(:f1<float>,:f2<char[31]>,:f3<char[2]>)", db);
    char tmp[32];
    char f3 = '1';

    for(int i=1;i<=100;++i){
        sprintf(tmp,"Name%d",i);
        o<< (float)i << tmp << f3;
    }
}

void select()
{ 
    otl_stream i(50, // buffer size
        "select f1, f2 from test_tab where f1>=:f<int> and f1<=:f*2",
        db // connect object
        ); 
    // create select stream

    float f1;
    char f2[31];

    i<<8; // assigning :f = 8

    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while(!i.eof()){ // while not end-of-data
        i>>f1>>f2;
        cout<<"f1="<<f1<<", f2="<<f2<<endl;
    }

    i<<4; // assigning :f = 4
    // SELECT automatically executes when all input variables are
    // assigned. First portion of output rows is fetched to the buffer

    while(!i.eof()){ // while not end-of-data
        i>>f1>>f2;
        cout<<"f1="<<f1<<", f2="<<f2<<endl;
    }

}


void testSelect()
{
    int count = 1;

    double f2;
    char f1[31];
    int f3;
    int idx = 0;
    int selCount = 0;
    for(idx = 0; idx < count; ++idx){
        otl_stream i(50, "select stock_code, market_code, report_code, stock_name, stocktype_code, " \
        "uplimited_ratio, trade_currency_no, face_values, status from stock t", db); 
        

        while(!i.eof()){ // while not end-of-data
            i>>f1>>f3>>f1>>f1>>f3
                >>f2>>f1>>f2>>f1;
            selCount++;
        }
    }
}


void testUpdate()
{ 
    int count = 100;

    int f2;
    char f1[31];
    int idx = 0;
    for(idx = 0; idx < count; ++idx){
        otl_stream o(50,  "update quote t set t.last_price = 5 where t.stock_code = '600400SH'", db);
    }
}

void testLongCall()
{
    DbGuarder* pDbGua = new DbGuarder(db);
    while(true){
        bool res = pDbGua->MakeSurePrepared();
        if(res){
            try{
                select();
            }catch(otl_exception& p){ // intercept OTL exceptions
                cerr<<p.msg<<endl; // print out error message
                cerr<<p.stm_text<<endl; // print out SQL that caused the error
                cerr<<p.var_info<<endl; // print out the variable that caused the error

                pDbGua->Except();
            }
        }else{
            cout << "except" << endl;
        }

        Sleep(1000);
    }

    delete pDbGua;
}

int main()
{
    otl_connect::otl_initialize(); // initialize OCI environment

    try{

        db.rlogon("hxasset/hxasset@myoracle"); // connect to Oracle

        cout << "connect to oracle server as user hxasset" << endl;
        //insert();
        //testSelect();
        testLongCall();
    }catch(otl_exception& p){ // intercept OTL exceptions

        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    db.logoff(); // disconnect from Oracle

    int i;
    cin >> i;

    return 0;

}
