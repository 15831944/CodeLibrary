// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <iostream>
#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32)
#include <windows.h>

#include "boot2.h"
#include "boot1.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "DLL_PROCESS_ATTACH" << std::endl;
        break;
    case DLL_THREAD_ATTACH:
        std::cout << "DLL_THREAD_ATTACH" << std::endl;
        break;
    case DLL_THREAD_DETACH:
        std::cout << "DLL_THREAD_DETACH" << std::endl;
        break;
    case DLL_PROCESS_DETACH:
        std::cout << "DLL_PROCESS_DETACH" << std::endl;
        break;
    }
    return TRUE;
}

#endif