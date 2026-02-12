// LibLanc Custom Memory Header
// Provides unique_ptr and related utilities without STL dependency
//
// This header provides a minimal implementation of unique_ptr and make_unique
// that can work with or without STL support. When __has_include(<memory>) is
// available and not disabled, it uses the standard library. Otherwise, it
// provides a compatible implementation.
//
// Attribution:
// The unique_ptr implementation is based on the C++ standard library specification
// and common implementation patterns. This is a simplified version that provides
// the essential functionality needed for this library.

#ifndef LIBLANC_MEMORY_H
#define LIBLANC_MEMORY_H

// Check if we should use standard library
#if defined(__has_include) && __has_include(<memory>) && !defined(LIBLANC_NO_STL)
    #define LIBLANC_USE_STD_MEMORY 1
    #include <memory>
#else
    #define LIBLANC_USE_STD_MEMORY 0
#endif

namespace LibLanc
{
namespace std
{

#if LIBLANC_USE_STD_MEMORY

// Use standard library implementations
using ::std::unique_ptr;
using ::std::make_unique;

#else

// Custom implementation when STL is not available
// Based on C++ standard library specification
// Reference: ISO/IEC 14882 (C++ Standard)

// Type traits needed for unique_ptr
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

template<typename T>
struct add_lvalue_reference
{
    typedef T& type;
};

template<typename T>
struct add_lvalue_reference<T&>
{
    typedef T& type;
};

// Forward function declarations
template<typename T>
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(typename remove_reference<T>::type&& t) noexcept
{
    return static_cast<T&&>(t);
}

// Default deleter for unique_ptr
template<typename T>
struct default_delete
{
    constexpr default_delete() noexcept = default;
    
    void operator()(T* ptr) const
    {
        delete ptr;
    }
};

// Specialization for arrays
template<typename T>
struct default_delete<T[]>
{
    constexpr default_delete() noexcept = default;
    
    void operator()(T* ptr) const
    {
        delete[] ptr;
    }
};

// Minimal unique_ptr implementation
template<typename T, typename Deleter = default_delete<T>>
class unique_ptr
{
public:
    using pointer = T*;
    using element_type = T;
    using deleter_type = Deleter;

    // Constructors
    constexpr unique_ptr() noexcept : _ptr(nullptr) {}
    constexpr unique_ptr(decltype(nullptr)) noexcept : _ptr(nullptr) {}
    
    explicit unique_ptr(pointer p) noexcept : _ptr(p) {}
    
    // Move constructor
    unique_ptr(unique_ptr&& other) noexcept : _ptr(other.release()) {}
    
    // Move assignment
    unique_ptr& operator=(unique_ptr&& other) noexcept
    {
        reset(other.release());
        return *this;
    }
    
    // Destructor
    ~unique_ptr()
    {
        if (_ptr != nullptr)
        {
            _deleter(_ptr);
        }
    }
    
    // Delete copy constructor and assignment
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
    
    // Modifiers
    pointer release() noexcept
    {
        pointer old = _ptr;
        _ptr = nullptr;
        return old;
    }
    
    void reset(pointer ptr = pointer()) noexcept
    {
        pointer old = _ptr;
        _ptr = ptr;
        if (old != nullptr)
        {
            _deleter(old);
        }
    }
    
    // Observers
    pointer get() const noexcept
    {
        return _ptr;
    }
    
    Deleter& get_deleter() noexcept
    {
        return _deleter;
    }
    
    const Deleter& get_deleter() const noexcept
    {
        return _deleter;
    }
    
    explicit operator bool() const noexcept
    {
        return _ptr != nullptr;
    }
    
    // Dereference operators
    typename add_lvalue_reference<T>::type operator*() const
    {
        return *_ptr;
    }
    
    pointer operator->() const noexcept
    {
        return _ptr;
    }

private:
    pointer _ptr;
    Deleter _deleter;
};

// make_unique implementation
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

#endif // LIBLANC_USE_STD_MEMORY

} // namespace std
} // namespace LibLanc

#endif // LIBLANC_MEMORY_H
