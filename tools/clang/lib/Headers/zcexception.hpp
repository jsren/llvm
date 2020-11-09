/* stdexcept.hpp - (c) 2018-2020 James Renwick */
#pragma once

#ifdef __cplusplus
#pragma clang system_header

extern "C" {
    void *aligned_alloc(decltype(sizeof(int)) alignment, decltype(sizeof(int)) size) throw ();
    void free(void* ptr) throw ();
}

alignas(64)
inline /*thread_local*/ unsigned char __exception_obj_buffer[64];

inline unsigned char* __exception_obj_ptr = __exception_obj_buffer;

inline unsigned char* __cxa_allocate_exception_obj(
    decltype(sizeof(int)) size, char alignment) noexcept
{
    if (__exception_obj_ptr + size > __exception_obj_buffer + sizeof(__exception_obj_buffer)) {
        return reinterpret_cast<unsigned char*>(::aligned_alloc(alignment, size));
    }
    // JSR TODO: Handle alignment correctly - this doesn't impact on WCET
    auto out = __exception_obj_ptr;
    __exception_obj_ptr += size;
    return out;
}

inline void __cxa_free_exception_obj(unsigned char* ptr, decltype(sizeof(int)) size, char alignment) noexcept {
    (void)alignment;
    if (ptr < __exception_obj_buffer || ptr > __exception_obj_buffer + sizeof(__exception_obj_buffer)) {
        ::free(ptr);
    }
    else __exception_obj_ptr -= size;
}

/**
 * The exception state object (ESO) class declaration.
 *
 * This must contain specific fields of specific types otherwise
 * clang will assert during compilation.
 */
struct __exception_t {
    // Address of exception object buffer
    unsigned char *buffer;
    // Address of list of base types
    const char** baseTypes;
    // Type id pointer
    void* type;
    // Size of exception object
    decltype(sizeof(int)) size;
    // If the original ctor is not 'throws', we will emit and assign a thunk instead
    // JSR TODO: support VTT pointer
    void(*ctor)(void*, __exception_t*, void*);
    void(*dtor)(void*);
    bool active;
    bool ptr;
    // Alignment of exception object
    char alignment;
};

/**
 * This struct provides additional constructors on top of the intentionally POD
 * __exception_t class.
 *
 * This facilitates 
 */
struct __exception_obj_base
{
    __exception_t exception{};

    ~__exception_obj_base() = default;
    constexpr __exception_obj_base(unsigned char* c, __exception_t e) noexcept
        : exception(e) { exception.buffer = c; }

    __exception_obj_base() = default;
    __exception_obj_base(const __exception_obj_base&) = default;
};

extern "C" {
    [[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
    extern const char* __typeid_empty_bases[1] = { nullptr };

    [[gnu::noinline]]
    inline bool __type_is_not_base(void* type, const char** super_bases) noexcept {
        for (; super_bases[0] != nullptr; super_bases++) {
            if (static_cast<const void*>(super_bases[0]) == type) return false;
        }
        return true;
    }
}

#endif
