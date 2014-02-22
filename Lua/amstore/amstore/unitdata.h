#ifndef _AM_UNITDATA_H
#define _AM_UNITDATA_H

#include <list>
#include <map>
#include "luautil/luna.h"
#include "luautil/luautil.h"
#include "config.h"

using std::list;
using std::map;

class UnitData{
public:
    static const char className[];
    typedef Lunar<UnitData>::RegType RegType;
    static RegType methods[];
public:
    UnitData():unit_id(0){
        cout << "UnitData construct" << endl;
    }
    UnitData(lua_State* L){
        new (this) UnitData();
        cout << "UnitData construct lua" << endl;
    }
    virtual ~UnitData(){
        cout << "UnitData destruct" << endl;
    }

    //��Ҫȷ������֮�󣬶�Ӧ��UnitData�ĵ�ַ����仯����lua�лᱣ����userdate
    static UnitData* Create(lua_State* L){
        UnitId_T id = PAI(L, 1);
        UnitData* pNew = GetByUnitId(id);
        if(!pNew){
            pNew = InitUnitData(id);
        }
        return pNew;
    }
private:
    //����unitid��ȡUnitData*�����޷���NULL
    static UnitData* GetByUnitId(const UnitId_T id);
    static UnitData* InitUnitData(const UnitId_T id);

    //unitid��UnitDataָ��Ĺ���
    static map<UnitId_T, UnitData*> _id2Data;
    //UnitData���б��ڳ��������ڼ䣬�����Ԫ�ش���֮�󲻻ᱻ�ͷź�ɾ�����Ա����ⲿ���е���ָ���userdata��Ч
    static list<UnitData*> _DataList;
private:
    UnitId_T unit_id;
};

#endif