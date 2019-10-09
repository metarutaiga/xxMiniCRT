//==============================================================================
// xxMiniCRT : MSVCRT minimal Header
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxMiniCRT
//==============================================================================
#pragma once

#ifndef _DEBUG

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" BOOL WINAPI _DllMainCRTStartup(HANDLE handle, DWORD reason, LPVOID preserved);
#define IMPLEMENT_MINICRT() \
extern "C" int wWinMainCRTStartup() \
{ \
    _DllMainCRTStartup(NULL, DLL_PROCESS_ATTACH, NULL); \
    int result = wWinMain(GetModuleHandle(NULL), NULL, L"", 0); \
    ExitProcess(result); \
    return result; \
}
#else
#define IMPLEMENT_MINICRT()
#endif
