#ifndef _BOOT2_H_
#define _BOOT2_H_

#include <iostream>
#include <string>

class Boot2{
public:
    Boot2(std::string m):msg(m){
        std::cout << "[construct]Boot2:" << msg << std::endl;
    };

    ~Boot2(){
        std::cout << "[destruct]~Boot2:" << msg << std::endl;
    }
private:
    std::string msg;

    static Boot2 _Instance_1;
    static Boot2 _Instance_2;
    static Boot2 _Instance_3;
};

static Boot2 GInstance2_h_1("global static variable (h_1)");
static Boot2 GInstance2_h_2("global static variable (h_2)");

#endif