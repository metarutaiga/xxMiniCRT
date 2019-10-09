//==============================================================================
// xxGraphic : MSVCRT minimal Source
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxGraphic
//==============================================================================
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef _INC_SETJMPEX
#define _INC_SETJMPEX
#endif
#include <Windows.h>
#include <float.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>

#ifndef _DEBUG

#if defined(_M_IX86)
#pragma comment(lib, "int64")
#endif
#pragma comment(linker, "/nodefaultlib:libcmt.lib")

//==============================================================================
//  new / delete
//==============================================================================
void* operator new(size_t size)
{
    return _aligned_malloc(size, 16);
}
void operator delete(void* ptr, size_t size)
{
    _aligned_free(ptr);
}
//==============================================================================
//  MSVCRT
//==============================================================================
extern "C" int _fltused = 1;
static HMODULE msvcrt = nullptr;
//------------------------------------------------------------------------------
#pragma section(".CRT$XCA", long, read)
#pragma section(".CRT$XCZ", long, read)
#pragma section(".CRT$XIA", long, read)
#pragma section(".CRT$XIZ", long, read)
typedef void (*_PVFV)();
extern "C" __declspec(allocate(".CRT$XCA")) const _PVFV __xc_a = 0;
extern "C" __declspec(allocate(".CRT$XCZ")) const _PVFV __xc_z = 0;
extern "C" __declspec(allocate(".CRT$XIA")) const _PVFV __xi_a = 0;
extern "C" __declspec(allocate(".CRT$XIZ")) const _PVFV __xi_z = 0;
#pragma comment(linker, "/merge:.CRT=.rdata")
//------------------------------------------------------------------------------
static void _initterm(const _PVFV* ppfn, const _PVFV* end)
{
    do
    {
        if (_PVFV pfn = *++ppfn)
        {
            pfn();
        }
    } while (ppfn < end);
}
//------------------------------------------------------------------------------
extern "C" BOOL WINAPI _DllMainCRTStartup(HANDLE handle, DWORD reason, LPVOID preserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        if (msvcrt == nullptr)
        {
#if defined(_M_IX86)
            msvcrt = LoadLibraryA("msvcrt20.dll");
#else
            msvcrt = LoadLibraryA("msvcrt.dll");
#endif
        }
        _initterm(&__xi_a, &__xi_z);
        _initterm(&__xc_a, &__xc_z);
        break;

    case DLL_PROCESS_DETACH:
        if (msvcrt)
        {
            FreeLibrary(msvcrt);
            msvcrt = nullptr;
        }
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
static FILE** __iob;
static int (*__vfprintf)(FILE*, char const*, va_list);
static int (*__vsnprintf)(char*, size_t, char const*, va_list);
static int (*__vsscanf)(char const*, char const*, va_list);
static float (*__sinf)(float);
//------------------------------------------------------------------------------
static FILE* acrt_iob_func(unsigned int i)
{
    return __iob[i];
}
//------------------------------------------------------------------------------
static int stdio_common_vfprintf(unsigned __int64 options, FILE* stream, char const* format, _locale_t locale, va_list argList)
{
    return __vfprintf(stream, format, argList);
}
//------------------------------------------------------------------------------
static int stdio_common_vsprintf(unsigned __int64 options, char* buffer, size_t bufferCount, char const* format, _locale_t locale, va_list argList)
{
    return __vsnprintf(buffer, bufferCount, format, argList);
}
//------------------------------------------------------------------------------
static int stdio_common_vsscanf(unsigned __int64 options, char const* buffer, size_t bufferCount, char const* format, _locale_t locale, va_list argList)
{
    return __vsscanf(buffer, format, argList);
}
//------------------------------------------------------------------------------
static float __fastcall libm_sse2_sincosf(float v)
{
    return __sinf(v);
}
//------------------------------------------------------------------------------
static void* aligned_malloc(size_t size, size_t alignment)
{
    size_t unalignedPtr = reinterpret_cast<size_t>(malloc(size + alignment));
    if (unalignedPtr == 0)
        return nullptr;

    char* alignedPtr = (char*)nullptr + ((unalignedPtr + alignment) & ~(alignment - 1));
    memcpy(alignedPtr - sizeof(size_t), &unalignedPtr, sizeof(size_t));
    return alignedPtr;
}
//------------------------------------------------------------------------------
static void* aligned_realloc(void* ptr, size_t size, size_t alignment)
{
    char* alignedPtr = (char*)ptr;
    if (alignedPtr == nullptr)
        return aligned_malloc(size, alignment);

    size_t unalignedPtr;
    memcpy(&unalignedPtr, alignedPtr - sizeof(size_t), sizeof(size_t));
    unalignedPtr = reinterpret_cast<size_t>(realloc((char*)nullptr + unalignedPtr, size + alignment));
    if (unalignedPtr == 0)
        return nullptr;

    alignedPtr = (char*)nullptr + ((unalignedPtr + alignment) & ~(alignment - 1));
    memcpy(alignedPtr - sizeof(size_t), &unalignedPtr, sizeof(size_t));
    return alignedPtr;
}
//------------------------------------------------------------------------------
static void aligned_free(void* ptr)
{
    char* alignedPtr = (char*)ptr;
    if (alignedPtr == nullptr)
        return;

    size_t unalignedPtr;
    memcpy(&unalignedPtr, alignedPtr - sizeof(size_t), sizeof(size_t));
    free((char*)nullptr + unalignedPtr);
}
//------------------------------------------------------------------------------
static void* getFunction(const char* name)
{
    if (name == "__acrt_iob_func")
    {
        if (__iob == nullptr)
        {
            (void*&)__iob = getFunction("_iob");
        }
        return acrt_iob_func;
    }
    if (name == "__stdio_common_vfprintf")
    {
        if (__vfprintf == nullptr)
        {
            (void*&)__vfprintf = getFunction("vfprintf");
        }
        return stdio_common_vfprintf;
    }
    if (name == "__stdio_common_vsprintf")
    {
        if (__vsnprintf == nullptr)
        {
            (void*&)__vsnprintf = getFunction("_vsnprintf");
        }
        return stdio_common_vsprintf;
    }
    if (name == "__stdio_common_vsscanf")
    {
        if (__vsnprintf == nullptr)
        {
            (void*&)__vsscanf = getFunction("vsscanf");
        }
        return stdio_common_vsscanf;
    }
    if (name == "__libm_sse2_sincosf_")
    {
        if (__sinf == nullptr)
        {
            (void*&)__sinf = getFunction("sinf");
        }
        return libm_sse2_sincosf;
    }

    if (name == "_aligned_malloc")
        return aligned_malloc;
    if (name == "_aligned_realloc")
        return aligned_realloc;
    if (name == "_aligned_free")
        return aligned_free;

    if (name == "setjmp")
        name = "_setjmp";

    void* function = nullptr;

    function = GetProcAddress(msvcrt, name);
    if (function)
        return function;

    static HMODULE user32 = nullptr;
    if (user32 == nullptr)
    {
        user32 = LoadLibraryA("user32.dll");
    }

    static int (WINAPI * MessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
    if (MessageBoxA == nullptr)
    {
        (void*&)MessageBoxA = GetProcAddress(user32, "MessageBoxA");
    }
    MessageBoxA(nullptr, name, "Microsoft Visual C++ Runtime Library", MB_OK);

    return nullptr;
}
//------------------------------------------------------------------------------
#if defined(_M_IX86)
#define FUNCTION(result, function, parameter, ...) \
static const char* xx ## function ## name = #function; \
static result (*xx ## function)(__VA_ARGS__); \
extern "C" __declspec(naked) result function(__VA_ARGS__) \
{ \
    __asm lea   ecx, xx ## function \
    __asm cmp   dword ptr [ecx], 0 \
    __asm jz    LOAD \
    __asm RUN: \
    __asm jmp   dword ptr [ecx] \
    __asm LOAD: \
    __asm push  ecx \
    __asm push  xx ## function ## name \
    __asm call  getFunction \
    __asm pop   ecx \
    __asm pop   ecx \
    __asm mov   [ecx], eax \
    __asm jmp   RUN \
}
#else
#define FUNCTION(result, function, parameter, ...) \
static const char* xx ## function ## name = #function; \
static result (*xx ## function)(__VA_ARGS__); \
extern "C" result function(__VA_ARGS__) \
{ \
    if (xx ## function == nullptr) \
    { \
        (void*&)xx ## function = getFunction(xx ## function ## name); \
    } \
    return xx ## function ## parameter; \
}
#endif
#pragma function(ceil)
#pragma function(floor)
#pragma function(pow)
#if defined(_M_AMD64)
#pragma function(asinf)
#pragma function(acosf)
#pragma function(atan2f)
#pragma function(sinf)
#pragma function(cosf)
#pragma function(powf)
#pragma function(fmodf)
#endif
#pragma function(memchr)
#pragma function(memcmp)
#pragma function(memcpy)
#pragma function(memmove)
#pragma function(memset)
#pragma function(strcmp)
FUNCTION(int,           atexit,                     (a),                void (*a)(void));
FUNCTION(double,        atof,                       (a),                char const* a);
FUNCTION(double,        ceil,                       (a),                double a);
FUNCTION(double,        floor,                      (a),                double a);
FUNCTION(double,        pow,                        (a, b),             double a, double b);
#if defined(_M_AMD64)
FUNCTION(float,         asinf,                      (a),                float a);
FUNCTION(float,         acosf,                      (a),                float a);
FUNCTION(float,         atan2f,                     (a, b),             float a, float b);
FUNCTION(float,         sinf,                       (a),                float a);
FUNCTION(float,         cosf,                       (a),                float a);
FUNCTION(float,         powf,                       (a, b),             float a, float b);
FUNCTION(float,         fmodf,                      (a, b),             float a, float b);
#endif
FUNCTION(void*,         calloc,                     (a, b),             size_t a, size_t b);
FUNCTION(void*,         malloc,                     (a),                size_t a);
FUNCTION(void*,         realloc,                    (a, b),             void* a, size_t b);
FUNCTION(void,          free,                       (a),                void* a);
FUNCTION(void*,         _aligned_malloc,            (a, b),             size_t a, size_t b);
FUNCTION(void*,         _aligned_realloc,           (a, b, c),          void* a, size_t b, size_t c);
FUNCTION(void,          _aligned_free,              (a),                void* a);
FUNCTION(int,           fclose,                     (a),                FILE* a);
FUNCTION(int,           fflush,                     (a),                FILE* a);
FUNCTION(FILE*,         fopen,                      (a, b),             char const* a, char const* b);
FUNCTION(size_t,        fread,                      (a, b, c, d),       void* a, size_t b, size_t c, FILE* d);
FUNCTION(int,           fseek,                      (a, b, c),          FILE* a, long b, int c);
FUNCTION(long,          ftell,                      (a),                FILE* a);
FUNCTION(size_t,        fwrite,                     (a, b, c, d),       void const* a, size_t b, size_t c, FILE* d);
FUNCTION(char*,         getenv,                     (a),                char const* a);
FUNCTION(void,          qsort,                      (a, b, c, d),       void* a, size_t b, size_t c, _CoreCrtNonSecureSearchSortCompareFunction d);
FUNCTION(void,          longjmp,                    (a, b),             jmp_buf a, int b);
FUNCTION(int,           setjmp,                     (a),                jmp_buf a);
FUNCTION(void const*,   memchr,                     (a, b, c),          void const* a, int b, size_t c);
FUNCTION(int,           memcmp,                     (a, b, c),          void const* a, void const* b, size_t c);
FUNCTION(void*,         memcpy,                     (a, b, c),          void* a, void const* b, size_t c);
FUNCTION(void*,         memmove,                    (a, b, c),          void* a, void const* b, size_t c);
FUNCTION(void*,         memset,                     (a, b, c),          void* a, int b, size_t c);
FUNCTION(char const*,   strchr,                     (a, b),             char const* a, int b);
FUNCTION(int,           strcmp,                     (a, b),             char const* a, char const* b);
FUNCTION(int,           strncmp,                    (a, b, c),          char const* a, char const* b, size_t c);
FUNCTION(char*,         strncpy,                    (a, b, c),          char* a, char const* b, size_t c);
FUNCTION(char const*,   strrchr,                    (a, b),             char const* a, int b);
FUNCTION(char const*,   strstr,                     (a, b),             char const* a, char const* b);
FUNCTION(long,          strtol,                     (a, b, c),          char const* a, char** b, int c);
FUNCTION(int,           toupper,                    (a),                int a);
FUNCTION(FILE*,         _wfopen,                    (a, b),             wchar_t const* a, wchar_t const* b);
FUNCTION(void,          _CIfmod,                    ());
FUNCTION(void,          _CIacos,                    ());
FUNCTION(void,          _CIpow,                     ());
FUNCTION(void,          __libm_sse2_sincosf_,       ());
FUNCTION(FILE*,         __acrt_iob_func,            (a),                unsigned a);
FUNCTION(int,           __stdio_common_vfprintf,    (a, b, c, d, e),    unsigned __int64 a, FILE* b, char const* c, _locale_t d, va_list e);
FUNCTION(int,           __stdio_common_vsprintf,    (a, b, c, d, e, f), unsigned __int64 a, char* b, size_t c, char const* d, _locale_t e, va_list f);
FUNCTION(int,           __stdio_common_vsscanf,     (a, b, c, d, e, f), unsigned __int64 a, char const* b, size_t c, char const* d, _locale_t e, va_list f);
//------------------------------------------------------------------------------
#if defined(_M_IX86)
extern "C" __declspec(naked) void _chkstk()
{
    __asm neg       eax
    __asm add       eax, esp
    __asm xchg      eax, esp
    __asm mov       eax, [eax]
    __asm mov       [esp], eax
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) void* _ftol2()
{
    __asm sub       esp, 12
    __asm fnstcw    [esp]
    __asm mov       ax, [esp]
    __asm or        ax, 0C00h
    __asm mov       [esp + 2], ax
    __asm fldcw     [esp + 2]
    __asm fistp     qword ptr [esp + 4]
    __asm fldcw     [esp]
    __asm pop       eax
    __asm pop       eax
    __asm pop       edx
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) void* _ftol2_sse()
{
    __asm sub       esp, 12
    __asm fnstcw    [esp]
    __asm mov       ax, [esp]
    __asm or        ax, 0C00h
    __asm mov       [esp + 2], ax
    __asm fldcw     [esp + 2]
    __asm fistp     qword ptr [esp + 4]
    __asm fldcw     [esp]
    __asm pop       eax
    __asm pop       eax
    __asm pop       edx
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) void _alloca_probe_16()
{
    __asm push      eax
    __asm lea       eax, [esp + 8]
    __asm sub       eax, [esp]
    __asm and       eax, 0Fh
    __asm add       eax, [esp]
    __asm add       esp, 4
    __asm jmp       _chkstk
}
#elif defined(_M_AMD64)
extern "C" void __chkstk()
{
}
#endif

#endif
