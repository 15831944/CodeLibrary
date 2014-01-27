#ifndef _BOOT1_H_
#define _BOOT1_H_

#include <iostream>
#include <string>

class Boot1{
public:
    Boot1(std::string m):msg(m){
        std::cout << "Boot1:" << msg << std::endl;
    };

    ~Boot1(){
        std::cout << "~Boot1:" << msg << std::endl;
    }
private:
    std::string msg;

    static Boot1 _Instance;
};

static Boot1 GInstance("global static variable (h)");

#endif