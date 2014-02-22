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

    //需要确保创建之后，对应的UnitData的地址不会变化，因lua中会保存其userdate
    static UnitData* Create(lua_State* L){
        UnitId_T id = PAI(L, 1);
        UnitData* pNew = GetByUnitId(id);
        if(!pNew){
            pNew = InitUnitData(id);
        }
        return pNew;
    }
private:
    //根据unitid获取UnitData*，若无返回NULL
    static UnitData* GetByUnitId(const UnitId_T id);
    static UnitData* InitUnitData(const UnitId_T id);

    //unitid到UnitData指针的关联
    static map<UnitId_T, UnitData*> _id2Data;
    //UnitData的列表，在程序运行期间，里面的元素创建之后不会被释放和删除，以保存外部持有的其指针或userdata有效
    static list<UnitData*> _DataList;
private:
    UnitId_T unit_id;
};

#endif