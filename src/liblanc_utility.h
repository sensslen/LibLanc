// LibLanc Custom Utility Header
// Provides move semantics without STL dependency
//
// This header provides move() and related utilities without requiring STL.
// When __has_include(<utility>) is available and not disabled, it uses the
// standard library. Otherwise, it provides a compatible implementation.
//
// Attribution:
// The move implementation is based on the C++ standard library specification.
// Reference: ISO/IEC 14882 (C++ Standard)

#ifndef LIBLANC_UTILITY_H
#define LIBLANC_UTILITY_H

// Check if we should use standard library
#if defined(__has_include) && __has_include(<utility>) && !defined(LIBLANC_NO_STL)
    #define LIBLANC_USE_STD_UTILITY 1
    #include <utility>
#else
    #define LIBLANC_USE_STD_UTILITY 0
#endif

namespace liblanc
{

#if LIBLANC_USE_STD_UTILITY

// Use standard library implementations
using std::move;

#else

// Custom implementation when STL is not available
// Based on C++ standard library specification
// Reference: ISO/IEC 14882 (C++ Standard)

// Type traits for move
template<typename T>
struct remove_reference
{
    typedef T type;
};

template<typename T>
struct remove_reference<T&>
{
    typedef T type;
};

template<typename T>
struct remove_reference<T&&>
{
    typedef T type;
};

// move implementation
template<typename T>
constexpr typename remove_reference<T>::type&& move(T&& t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(t);
}

#endif // LIBLANC_USE_STD_UTILITY

} // namespace liblanc

#endif // LIBLANC_UTILITY_H
