#include "dbguarder.h"

int DbGuarder::Except()
{
    //判断是否还在连接
    bool isConnected = true;
    try{
        dbConn.rollback();
    }catch(otl_exception& p){
        std::cerr<<p.msg<<std::endl; 
        std::cerr<<p.stm_text<<std::endl; 
        std::cerr<<p.var_info<<std::endl; 
        isConnected = false;
    }

    if(!isConnected){
        try{
            flag = DF_Except;
            dbConn.logoff();
        }catch(otl_exception& p){
            flag = DF_Except;
            std::cerr<<p.msg<<std::endl; 
            std::cerr<<p.stm_text<<std::endl; 
            std::cerr<<p.var_info<<std::endl; 
            return false;
        }
    }else{
        //若仍然还在连接，则不做更多事
    }
    return 0;
}


bool DbGuarder::MakeSurePrepared()
{
    if(DF_Inited == flag || DF_OK == flag){
        return true;
    }else{
        //判断现在是否已经正常
        int val = 0;
        try{
            char sql[] = "select 1 from dual";
            otl_stream ostr(1,  sql, dbConn); 
            ostr >> val;
        }catch(otl_exception& p){
            cerr<<p.msg<<endl;
            cerr<<p.stm_text<<endl;
            cerr<<p.var_info<<endl;
        }

        if(val != 1){
            try{
                dbConn.rlogon("hxasset/hxasset@myoracle");
                dbConn.auto_commit_off();
            }catch(otl_exception& p){ 
                std::cerr << "msg=" << p.msg<<std::endl; 
                std::cerr << "stm_text=" <<p.stm_text<<std::endl; 
                std::cerr << "var_info=" <<p.var_info<<std::endl; 
                return false;
            }
        }
        
        flag = DF_OK;
        return true;
    }
}