// LibLanc Custom CString Header
// Provides memset and other C string functions without STL dependency
//
// This header provides memset() without requiring STL <cstring>.
// When __has_include(<cstring>) is available and not disabled, it uses the
// standard library. Otherwise, it provides a compatible implementation.
//
// Attribution:
// The memset implementation is a standard C library function.
// This provides a fallback when <cstring> is not available.

#ifndef LIBLANC_CSTRING_H
#define LIBLANC_CSTRING_H

#include <stddef.h> // for size_t

// Check if we should use standard library
#if defined(__has_include) && __has_include(<cstring>) && !defined(LIBLANC_NO_STL)
    #define LIBLANC_USE_STD_CSTRING 1
    #include <cstring>
#else
    #define LIBLANC_USE_STD_CSTRING 0
#endif

namespace liblanc
{

#if LIBLANC_USE_STD_CSTRING

// Use standard library implementation
using std::memset;

#else

// Custom implementation when STL is not available
// memset is a standard C library function that fills a block of memory
// with a specified value.
// Reference: ISO/IEC 9899 (C Standard)

inline void* memset(void* ptr, int value, size_t num)
{
    unsigned char* p = static_cast<unsigned char*>(ptr);
    unsigned char val = static_cast<unsigned char>(value);
    
    for (size_t i = 0; i < num; ++i)
    {
        p[i] = val;
    }
    
    return ptr;
}

#endif // LIBLANC_USE_STD_CSTRING

} // namespace liblanc

#endif // LIBLANC_CSTRING_H
