#ifndef _BOOT1_H_
#define _BOOT1_H_

#include <iostream>
#include <string>

class Boot1{
public:
    Boot1(std::string m):msg(m){
        std::cout << "[construct]Boot1:" << msg << std::endl;
    };

    ~Boot1(){
        std::cout << "[destruct]~Boot1:" << msg << std::endl;
    }
private:
    std::string msg;

    static Boot1 _Instance_1;
    static Boot1 _Instance_2;
    static Boot1 _Instance_3;
};

static Boot1 GInstance1_h_1("global static variable (h_1)");
static Boot1 GInstance1_h_2("global static variable (h_2)");

#endif