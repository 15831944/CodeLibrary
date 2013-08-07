#include <iostream>
#include <Windows.h>
#include "Timer.h"

using namespace std;

int main()
{

    TimerNano timer;
    TimerNano timerBegin;

    GetTimeOnNano(&timerBegin);
    double flag1 = GetCurrentTimerFlag();
    for(int i = 0; i < 10000; ++i){
        GetTimeOnNano(&timer);
    }
    GetTimeOnNano(&timer);
    double flag2 = GetCurrentTimerFlag();
    
    PrintTimeNano(timerBegin);
    PrintTimeNano(timer);
    cout.precision(20);
    cout << "flag1:" << flag1 << ", flag2:" << flag2 << ", diff" << (flag2 - flag1)
        << ", pre: " << ((flag2 - flag1)*1000000/10000) << endl;

    int i;
    std::cin >> i;
    return 0;
}