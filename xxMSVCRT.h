//==============================================================================
// xxMiniCRT : MSVCRT minimal Header
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxMiniCRT
//==============================================================================
#pragma once

#if !defined(_DEBUG) && !defined(__llvm__)

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(_M_IX86)
#define IMPLEMENT_MINICRT_LIB() \
__pragma(comment(lib, "xxMiniCRT.Release.x86.lib"))
#elif defined(_M_AMD64)
#define IMPLEMENT_MINICRT_LIB() \
__pragma(comment(lib, "xxMiniCRT.Release.x64.lib"))
#elif defined(_M_ARM)
#define IMPLEMENT_MINICRT_LIB() \
__pragma(comment(lib, "xxMiniCRT.Release.ARM.lib"))
#elif defined(_M_ARM64)
#define IMPLEMENT_MINICRT_LIB() \
__pragma(comment(lib, "xxMiniCRT.Release.ARM64.lib"))
#endif

extern "C" IMAGE_DOS_HEADER __ImageBase;
extern "C" BOOL WINAPI _DllMainCRTStartup(HANDLE handle, DWORD reason, LPVOID preserved);
#define IMPLEMENT_MINICRT() \
IMPLEMENT_MINICRT_LIB() \
__pragma(comment(linker, "/nodefaultlib:libcmt.lib")) \
__pragma(comment(linker, "/nodefaultlib:libcpmt.lib")) \
__pragma(comment(linker, "/nodefaultlib:msvcrt.lib")) \
__pragma(comment(linker, "/nodefaultlib:msvcprt.lib")) \
extern "C" void wWinMainCRTStartup() \
{ \
    _DllMainCRTStartup(NULL, DLL_PROCESS_ATTACH, NULL); \
    STARTUPINFOW startupInfo; \
    GetStartupInfoW(&startupInfo); \
    int showWindowMode = startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT; \
    int result = wWinMain((HINSTANCE)&__ImageBase, NULL, GetCommandLineW(), showWindowMode); \
    _DllMainCRTStartup(NULL, DLL_PROCESS_DETACH, NULL); \
    TerminateProcess(GetCurrentProcess(), result); \
}
#else
#define IMPLEMENT_MINICRT()
#endif
