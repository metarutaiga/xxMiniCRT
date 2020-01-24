//==============================================================================
// xxMiniCRT : MSVCRT minimal Source
//
// Copyright (c) 2019 TAiGA
// https://github.com/metarutaiga/xxMiniCRT
//==============================================================================
#ifndef _INC_SETJMPEX
#   define _INC_SETJMPEX
#endif
#ifndef _HAS_EXCEPTIONS
#   define _HAS_EXCEPTIONS 0
#endif
#include <float.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <exception>

#if !defined(_DEBUG) && !defined(__llvm__)

#include "xxMSVCRT.h"

#pragma warning(disable:4244)
#pragma warning(disable:4645)
#if defined(_M_IX86)
#   pragma comment(lib, "int64")
#endif
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:libcpmt.lib")
#pragma comment(linker, "/nodefaultlib:msvcrt.lib")
#pragma comment(linker, "/nodefaultlib:msvcprt.lib")

//==============================================================================
//  new / delete
//==============================================================================
void* operator new(size_t size)
{
#if defined(_M_AMD64)
    return malloc(size);
#else
    return _aligned_malloc(size, 16);
#endif
}
void* operator new[](size_t size)
{
#if defined(_M_AMD64)
    return malloc(size);
#else
    return _aligned_malloc(size, 16);
#endif
}
//------------------------------------------------------------------------------
void operator delete(void* ptr, size_t size)
{
#if defined(_M_AMD64)
    free(ptr);
#else
    _aligned_free(ptr);
#endif
}
void operator delete[](void* ptr)
{
#if defined(_M_AMD64)
    free(ptr);
#else
    _aligned_free(ptr);
#endif
}
void operator delete[](void* ptr, size_t size)
{
#if defined(_M_AMD64)
    free(ptr);
#else
    _aligned_free(ptr);
#endif
}
//------------------------------------------------------------------------------
_STD_BEGIN
_Prhand _Raise_handler;
void _Xlength_error(const char*)
{
    __debugbreak();
}
_STD_END
//------------------------------------------------------------------------------
extern "C" void _invalid_parameter_noinfo_noreturn()
{
    __debugbreak();
}
//------------------------------------------------------------------------------
extern "C" void _invoke_watson(wchar_t const* const expression, wchar_t const* const function_name, wchar_t const* const file_name, unsigned int const line_number, uintptr_t const reserved)
{
    __debugbreak();
}
//==============================================================================
//  MSVCRT
//==============================================================================
extern "C" const int _fltused = 1;
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
#pragma comment(linker, "/merge:.rdata=.text")
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
        _initterm(&__xi_a, &__xi_z);
        _initterm(&__xc_a, &__xc_z);
        break;

    case DLL_PROCESS_DETACH:
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
//==============================================================================
//  wrapper
//==============================================================================
static FILE** __iob;
static FILE* acrt_iob_func(unsigned int i)
{
    return __iob[i];
}
//------------------------------------------------------------------------------
static int (*__vfprintf)(FILE*, char const*, va_list);
static int stdio_common_vfprintf(unsigned __int64 options, FILE* stream, char const* format, _locale_t locale, va_list argList)
{
    return __vfprintf(stream, format, argList);
}
//------------------------------------------------------------------------------
static int (*__vsnprintf)(char*, size_t, char const*, va_list);
static int stdio_common_vsprintf(unsigned __int64 options, char* buffer, size_t bufferCount, char const* format, _locale_t locale, va_list argList)
{
    return __vsnprintf(buffer, bufferCount, format, argList);
}
//------------------------------------------------------------------------------
static int (*__vsscanf)(char const*, char const*, va_list);
static int stdio_common_vsscanf(unsigned __int64 options, char const* buffer, size_t bufferCount, char const* format, _locale_t locale, va_list argList)
{
    return __vsscanf(buffer, format, argList);
}
//------------------------------------------------------------------------------
#if defined(_M_AMD64)
static float (*__sinf)(float);
static float (*__cosf)(float);
static float (*__acosf)(float);
static float (*__powf)(float, float);
#endif
static double (*__sin)(double);
static double (*__cos)(double);
static double (*__acos)(double);
static double (*__pow)(double, double);
static __m128 __vectorcall libm_sse2_sinf(float f)
{
    __m128 v;
#if defined(__llvm__)
#   if defined(_M_AMD64)
    v[0] = __sinf(f);
#   else
    v[0] = __sin(f);
#   endif
#else
#   if defined(_M_AMD64)
    v.m128_f32[0] = __sinf(f);
#   else
    v.m128_f32[0] = __sin(f);
#   endif
#endif
    return v;
}
static __m128 __vectorcall libm_sse2_cosf(float f)
{
    __m128 v;
#if defined(__llvm__)
#   if defined(_M_AMD64)
    v[0] = __cosf(f);
#   else
    v[0] = __cos(f);
#   endif
#else
#   if defined(_M_AMD64)
    v.m128_f32[0] = __cosf(f);
#   else
    v.m128_f32[0] = __cos(f);
#   endif
#endif
    return v;
}
static __m128 __vectorcall libm_sse2_sincosf(float f)
{
    __m128 v;
#if defined(__llvm__)
#   if defined(_M_AMD64)
    v[0] = __sinf(f);
    v[1] = __cosf(f);
#   else
    v[0] = __sin(f);
    v[1] = __cos(f);
#   endif
#else
#   if defined(_M_AMD64)
    v.m128_f32[0] = __sinf(f);
    v.m128_f32[1] = __cosf(f);
#   else
    v.m128_f32[0] = __sin(f);
    v.m128_f32[1] = __cos(f);
#   endif
#endif
    return v;
}
static __m128 __vectorcall libm_sse2_acosf(float f)
{
    __m128 v;
#if defined(__llvm__)
#   if defined(_M_AMD64)
    v[0] = __acosf(f);
#   else
    v[0] = __acos(f);
#   endif
#else
#   if defined(_M_AMD64)
    v.m128_f32[0] = __acosf(f);
#   else
    v.m128_f32[0] = __acos(f);
#   endif
#endif
    return v;
}
static __m128d __vectorcall libm_sse2_pow(float a, double b)
{
    __m128d v;
#if defined(__llvm__)
    v[0] = __pow(a, b);
#else
    v.m128d_f64[0] = __pow(a, b);
#endif
    return v;
}
static __m128 __vectorcall libm_sse2_powf(float a, float b)
{
    __m128 v;
#if defined(__llvm__)
#   if defined(_M_AMD64)
    v[0] = __powf(a, b);
#   else
    v[0] = __pow(a, b);
#   endif
#else
#   if defined(_M_AMD64)
    v.m128_f32[0] = __powf(a, b);
#   else
    v.m128_f32[0] = __pow(a, b);
#   endif
#endif
    return v;
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
static void* getFunction(const char* name, bool assert = true)
{
    if (name == nullptr)
        return nullptr;

    static HMODULE msvcrt = nullptr;
    if (msvcrt == nullptr)
        msvcrt = LoadLibraryA("msvcrt.dll");
#if defined(_M_IX86) && 0
    if (msvcrt == nullptr)
        msvcrt = LoadLibraryA("msvcrt40.dll");
    if (msvcrt == nullptr)
        msvcrt = LoadLibraryA("msvcrt20.dll");
#endif
    if (msvcrt == nullptr)
        return nullptr;

    void* function = GetProcAddress(msvcrt, name);
    if (function)
        return function;

    if (assert == false)
        return nullptr;

    static HMODULE user32 = nullptr;
    if (user32 == nullptr)
        user32 = LoadLibraryA("user32.dll");
    if (user32 == nullptr)
        return nullptr;

    static int (WINAPI * MessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
    if (MessageBoxA == nullptr)
        (void*&)MessageBoxA = GetProcAddress(user32, "MessageBoxA");
    if (MessageBoxA == nullptr)
        return nullptr;
    MessageBoxA(nullptr, name, "Microsoft Visual C++ Runtime Library", MB_OK);

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__acrt_iob_func()
{
    if (__iob == nullptr)
        (void*&)__iob = getFunction("_iob");
    if (__iob)
        return acrt_iob_func;

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction_aligned_malloc()
{
    void* function = getFunction("_aligned_malloc", false);
    if (function)
        return function;

    return aligned_malloc;
}
//------------------------------------------------------------------------------
static void* getFunction_aligned_realloc()
{
    void* function = getFunction("_aligned_realloc", false);
    if (function)
        return function;

    return aligned_realloc;
}
//------------------------------------------------------------------------------
static void* getFunction_aligned_free()
{
    void* function = getFunction("_aligned_free", false);
    if (function)
        return function;

    return aligned_free;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_sinf()
{
#if defined(_M_AMD64)
    if (__sinf == nullptr)
        (void*&)__sinf = getFunction("sinf");
    if (__sinf)
        return libm_sse2_sinf;
#else
    if (__sin == nullptr)
        (void*&)__sin = getFunction("sin");
    if (__sin)
        return libm_sse2_sinf;
#endif

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_cosf()
{
#if defined(_M_AMD64)
    if (__cosf == nullptr)
        (void*&)__cosf = getFunction("cosf");
    if (__cosf)
        return libm_sse2_cosf;
#else
    if (__cos == nullptr)
        (void*&)__cos = getFunction("cos");
    if (__cos)
        return libm_sse2_cosf;
#endif

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_sincosf_()
{
#if defined(_M_AMD64)
    if (__sinf == nullptr)
        (void*&)__sinf = getFunction("sinf");
    if (__cosf == nullptr)
        (void*&)__cosf = getFunction("cosf");
    if (__sinf && __cosf)
        return libm_sse2_sincosf;
#else
    if (__sin == nullptr)
        (void*&)__sin = getFunction("sin");
    if (__cos == nullptr)
        (void*&)__cos = getFunction("cos");
    if (__sin && __cos)
        return libm_sse2_sincosf;
#endif

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_acosf()
{
#if defined(_M_AMD64)
    if (__acosf == nullptr)
        (void*&)__acosf = getFunction("acosf");
    if (__acosf)
        return libm_sse2_acosf;
#else
    if (__acos == nullptr)
        (void*&)__acos = getFunction("acos");
    if (__acos)
        return libm_sse2_acosf;
#endif

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_pow()
{
    if (__pow == nullptr)
        (void*&)__pow = getFunction("pow");
    if (__pow)
        return libm_sse2_pow;

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__libm_sse2_powf()
{
#if defined(_M_AMD64)
    if (__powf == nullptr)
        (void*&)__powf = getFunction("powf");
    if (__powf)
        return libm_sse2_powf;
#else
    if (__pow == nullptr)
        (void*&)__pow = getFunction("pow");
    if (__pow)
        return libm_sse2_powf;
#endif

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunctionsetjmp()
{
    return getFunction("_setjmp");
}
//------------------------------------------------------------------------------
static void* getFunction__stdio_common_vfprintf()
{
    if (__vfprintf == nullptr)
        (void*&)__vfprintf = getFunction("vfprintf");
    if (__vfprintf)
        return stdio_common_vfprintf;

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__stdio_common_vsprintf()
{
    if (__vsnprintf == nullptr)
        (void*&)__vsnprintf = getFunction("_vsnprintf");
    if (__vsnprintf)
        return stdio_common_vsprintf;

    return nullptr;
}
//------------------------------------------------------------------------------
static void* getFunction__stdio_common_vsscanf()
{
    if (__vsnprintf == nullptr)
        (void*&)__vsscanf = getFunction("vsscanf");
    if (__vsnprintf)
        return stdio_common_vsscanf;

    return nullptr;
}
//------------------------------------------------------------------------------
#define FUNCTION(result, function, parameter, ...) \
extern "C" result function(__VA_ARGS__) \
{ \
    static result (*xx ## function)(__VA_ARGS__); \
    if (xx ## function == nullptr) \
    { \
        static const char* const xx ## function ## name = #function; \
        (void*&)xx ## function = getFunction(xx ## function ## name); \
    } \
    return xx ## function parameter; \
}
//------------------------------------------------------------------------------
#define FUNCTION1(result, function, parameter, ...) \
extern "C" result function(__VA_ARGS__) \
{ \
    static result (*xx ## function)(__VA_ARGS__); \
    if (xx ## function == nullptr) \
    { \
        (void*&)xx ## function = getFunction ## function(); \
    } \
    return xx ## function parameter; \
}
//------------------------------------------------------------------------------
#pragma function(acos)
#pragma function(atan2)
#pragma function(ceil)
#pragma function(cos)
#pragma function(floor)
#pragma function(fmod)
#pragma function(pow)
#pragma function(sin)
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
#pragma function(strcat)
#pragma function(strcmp)
#pragma function(strcpy)
#pragma function(strlen)
FUNCTION(int,           atexit,                     (a),                void (*a)(void));
FUNCTION(double,        acos,                       (a),                double a);
FUNCTION(double,        atan2,                      (a, b),             double a, double b);
FUNCTION(double,        atof,                       (a),                char const* a);
FUNCTION(double,        ceil,                       (a),                double a);
FUNCTION(double,        cos,                        (a),                double a);
FUNCTION(double,        floor,                      (a),                double a);
FUNCTION(double,        fmod,                       (a, b),             double a, double b);
FUNCTION(double,        pow,                        (a, b),             double a, double b);
FUNCTION(double,        sin,                        (a),                double a);
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
FUNCTION1(void*,        _aligned_malloc,            (a, b),             size_t a, size_t b);
FUNCTION1(void*,        _aligned_realloc,           (a, b, c),          void* a, size_t b, size_t c);
FUNCTION1(void,         _aligned_free,              (a),                void* a);
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
FUNCTION1(int,          setjmp,                     (a),                jmp_buf a);
FUNCTION(void const*,   memchr,                     (a, b, c),          void const* a, int b, size_t c);
FUNCTION(int,           memcmp,                     (a, b, c),          void const* a, void const* b, size_t c);
FUNCTION(void*,         memcpy,                     (a, b, c),          void* a, void const* b, size_t c);
FUNCTION(void*,         memmove,                    (a, b, c),          void* a, void const* b, size_t c);
FUNCTION(void*,         memset,                     (a, b, c),          void* a, int b, size_t c);
FUNCTION(int,           remove,                     (a),                char const* a);
FUNCTION(char*,         strcat,                     (a, b),             char* a, char const* b);
FUNCTION(char const*,   strchr,                     (a, b),             char const* a, int b);
FUNCTION(int,           strcmp,                     (a, b),             char const* a, char const* b);
FUNCTION(char*,         strcpy,                     (a, b),             char* a, char const* b);
FUNCTION(char*,         _strdup,                    (a),                char const* a);
FUNCTION(size_t,        strlen,                     (a),                char const* a);
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
FUNCTION1(void,         __libm_sse2_acosf,          ());
FUNCTION1(void,         __libm_sse2_cosf,           ());
FUNCTION1(void,         __libm_sse2_sinf,           ());
FUNCTION1(void,         __libm_sse2_sincosf_,       ());
FUNCTION1(void,         __libm_sse2_pow,            ());
FUNCTION1(void,         __libm_sse2_powf,           ());
FUNCTION1(FILE*,        __acrt_iob_func,            (a),                unsigned a);
FUNCTION1(int,          __stdio_common_vfprintf,    (a, b, c, d, e),    unsigned __int64 a, FILE* b, char const* c, _locale_t d, va_list e);
FUNCTION1(int,          __stdio_common_vsprintf,    (a, b, c, d, e, f), unsigned __int64 a, char* b, size_t c, char const* d, _locale_t e, va_list f);
FUNCTION1(int,          __stdio_common_vsscanf,     (a, b, c, d, e, f), unsigned __int64 a, char const* b, size_t c, char const* d, _locale_t e, va_list f);
//------------------------------------------------------------------------------
#if defined(_M_IX86)
//------------------------------------------------------------------------------
extern "C" static const double TrickDouble = { 6755399441055744.0 };
extern "C" static const double _TrickDouble = { 6755399441055744.0 };
extern "C" static const double Int32ToUInt32[] = { 0.0, 4294967296.0 }; 
extern "C" static const double _Int32ToUInt32[] = { 0.0, 4294967296.0 };
//------------------------------------------------------------------------------
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
extern "C" __declspec(naked) long long _ftol2(float f)
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
extern "C" __declspec(naked) long long _ftol2_sse(float f)
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
extern "C" __declspec(naked) long long _ftol2_sse_excpt(float f)
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
extern "C" __declspec(naked) long long _ftol3(float f)
{
    __asm cvttss2si eax, xmm0
    __asm cdq
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) unsigned int _ftoui3(float f)
{
    __asm cvttss2si eax, xmm0
    __asm test      eax, 080000000h
    __asm jnz       _ftoui3_neg
    __asm ret
    __asm _ftoui3_neg:
    __asm xor       eax, eax
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) unsigned long long _ftoul3(float f)
{
    __asm cvttss2si eax, xmm0
    __asm test      eax, 080000000h
    __asm jnz       _ftoul3_neg
    __asm cdq
    __asm ret
    __asm _ftoul3_neg:
    __asm xor       eax, eax
    __asm xor       edx, edx
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) long long _dtol3(double d)
{
    __asm sub       esp, 8
    __asm movsd     xmm1, _TrickDouble
    __asm addsd     xmm0, xmm1
    __asm psubq     xmm0, xmm1
    __asm movq      qword ptr [esp], xmm0
    __asm pop       eax
    __asm pop       edx
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) unsigned int _dtoui3(double d)
{
    __asm cvttsd2si eax, xmm0
    __asm test      eax, 080000000h
    __asm jnz       _dtoui3_neg
    __asm ret
    __asm _dtoui3_neg:
    __asm xor       eax, eax
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) unsigned long long _dtoul3(double d)
{
    __asm sub       esp, 8
    __asm movsd     xmm1, _TrickDouble
    __asm addsd     xmm0, xmm1
    __asm pxor      xmm0, xmm1
    __asm movq      qword ptr [esp], xmm0
    __asm pop       eax
    __asm pop       edx
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) double _ltod3(long long l)
{
    __asm cvtsi2sd  xmm0, ecx
    __asm shr       ecx, 31
    __asm cvtsi2sd  xmm1, edx
    __asm addsd     xmm0, _Int32ToUInt32[ecx * 8]
    __asm mulsd     xmm1, _Int32ToUInt32[8]
    __asm addsd     xmm0, xmm1
    __asm ret
}
//------------------------------------------------------------------------------
extern "C" __declspec(naked) double _ultod3(unsigned long long l)
{
    __asm cvtsi2sd  xmm0, ecx
    __asm shr       ecx, 31
    __asm cvtsi2sd  xmm1, edx
    __asm shr       edx, 31
    __asm addsd     xmm0, _Int32ToUInt32[ecx * 8]
    __asm addsd     xmm1, _Int32ToUInt32[edx * 8]
    __asm mulsd     xmm1, _Int32ToUInt32[8]
    __asm addsd     xmm0, xmm1
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
