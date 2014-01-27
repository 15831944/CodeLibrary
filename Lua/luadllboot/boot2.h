#ifndef _BOOT2_H_
#define _BOOT2_H_

#include <iostream>
#include <string>

class Boot2{
public:
    Boot2(std::string m):msg(m){
        std::cout << "Boot2:" << msg << std::endl;
    };

    ~Boot2(){
        std::cout << "~Boot2:" << msg << std::endl;
    }
private:
    std::string msg;

    static Boot2 _Instance;
};

static Boot2 GInstance_2h("global static variable (h)");

#endif