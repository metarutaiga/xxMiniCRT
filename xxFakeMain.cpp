//==============================================================================
// xxMiniCRT : MSVCRT FakeMain Source
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxMiniCRT
//==============================================================================
#include <windows.h>
#include <shellapi.h>
#include <malloc.h>

extern "C" BOOL WINAPI _DllMainCRTStartup(HANDLE handle, DWORD reason, LPVOID preserved);
extern "C" void __getmainargs(int*, char***, char***, int, int*);
extern "C" void __set_app_type(int);
extern "C" int main(int, char**, char**);

__pragma(comment(linker, "/nodefaultlib:libcmt.lib"))
__pragma(comment(linker, "/nodefaultlib:libcpmt.lib"))
__pragma(comment(linker, "/nodefaultlib:msvcrt.lib"))
__pragma(comment(linker, "/nodefaultlib:msvcprt.lib"))

extern "C" int mainCRTStartup()
{
    _DllMainCRTStartup(NULL, DLL_PROCESS_ATTACH, NULL);
    int argc;
    char** argv;
    char** envp;
    int startupinfo = 0;
    __set_app_type(1);
    __getmainargs(&argc, &argv, &envp, 0, &startupinfo);
    int result = main(argc, argv, envp);
    ExitProcess(result); 
    return result;
}
