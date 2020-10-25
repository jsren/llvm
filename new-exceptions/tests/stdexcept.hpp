/* stdexcept.hpp - (c) 2018 James Renwick */
#pragma once
#include <stdlib.h>

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

//static_assert(std::is_trivially_copyable<__exception_t>::value,"");
//static_assert(std::is_trivially_destructible<__exception_t>::value,"");


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
// reinterpret_cast<__exception_obj_base*>(__builtin_get_exception_obj()) must be safe
//static_assert(std::is_trivial_layout<__exception_obj_base>::value, "");


extern "C" {
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_nullptr_t = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_int = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_1int = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_bool = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_EmptyObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_TrivialObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_NoTrivialObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_NoTrivialDtorObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_NoTrivialMoveObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_NoMoveObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_CtorThrowableObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_MoveThrowableObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_CtorThrowsObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_MoveThrowsObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_DtorThrowsObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_BaseObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_1BaseObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_SuperObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_1SuperObj = 0;
    // [[gnu::weak, gnu::section(".typeids")]] alignas(1)
    //     extern const char __typeid_for_DtorTestObj = 0;

    [[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
    extern const char* __typeid_empty_bases[1] = { nullptr };

    // Alternative form
    // [[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
    // extern const char* __typeid_bases_for_SuperObj[] = {
    //     &__typeid_for_BaseObj,
    //     nullptr
    // };

    [[gnu::noinline]]
    inline bool __type_is_not_base(void* type, const char** super_bases) noexcept {
        for (; super_bases[0] != nullptr; super_bases++) {
            if (static_cast<const void*>(super_bases[0]) == type) return false;
        }
        return true;
    }
}

struct EmptyObj { };
struct TrivialObj {
    int i;
};
struct NoTrivialObj {
    int i;
    NoTrivialObj(int i) : i(i) { }
    NoTrivialObj(const NoTrivialObj& o) : i(o.i) { }
    NoTrivialObj(NoTrivialObj&& o) : i(o.i) { }
    ~NoTrivialObj() { }
};
struct NoTrivialDtor {
    int i;
    NoTrivialDtor(int i) : i(i) { }
    NoTrivialDtor(const NoTrivialDtor&) = default;
    NoTrivialDtor(NoTrivialDtor&& o) = default;
    ~NoTrivialDtor() { }
};
struct NoTrivialMove {
    int i;
    NoTrivialMove(int i) : i(i) { }
    NoTrivialMove(const NoTrivialMove&) = delete;
    NoTrivialMove(NoTrivialMove&& o) : i(o.i) { }
    ~NoTrivialMove() = default;
};
struct NoMoveObj {
    int i;
    NoMoveObj(const NoMoveObj&) = delete;
    NoMoveObj(NoMoveObj&&) = delete;
};
struct CtorThrowableObj {
    int i;
    CtorThrowableObj(int i) throws : i(i) { }
    CtorThrowableObj(const CtorThrowableObj&) = default;
    CtorThrowableObj(CtorThrowableObj&&) = default;
    ~CtorThrowableObj() = default;
};
struct MoveThrowableObj {
    int i;
    MoveThrowableObj(int i) : i(i) { }
    MoveThrowableObj(const MoveThrowableObj&) throws = delete;
    MoveThrowableObj(MoveThrowableObj&& o) throws : i(o.i) { }
    ~MoveThrowableObj() = default;
};
struct CtorThrowsObj {
    int i;
    CtorThrowsObj(int i) throws : i(i) { throw 0; }
    CtorThrowsObj(const CtorThrowsObj&) = default;
    CtorThrowsObj(CtorThrowsObj&&) = default;
    ~CtorThrowsObj() = default;
};
struct MoveThrowsObj {
    int i;
    MoveThrowsObj(int i) : i(i) { }
    MoveThrowsObj(const MoveThrowsObj& o) throws : i(o.i) { throw 3; };
    MoveThrowsObj(MoveThrowsObj&& o) throws : i(o.i) { throw 3; };
    ~MoveThrowsObj() = default;
};
struct DtorThrowsObj {
    int i;
    DtorThrowsObj(int i) : i(i) { }
    DtorThrowsObj(const DtorThrowsObj&) = default;
    DtorThrowsObj(DtorThrowsObj&&) = default;
    ~DtorThrowsObj() throws { throw i; }
};
struct BaseObj {
    int i = 2;
    virtual int value() const {
        return 1;
    }
};
struct SuperObj : BaseObj {
    virtual int value() const override {
        return 2;
    }
    SuperObj() = default;
    SuperObj(SuperObj&&) = default;
    SuperObj(const SuperObj&) = default;
};

inline void shit()
{
    SuperObj o;
    SuperObj o2{reinterpret_cast<SuperObj&&>(o)};
}

struct DtorTestObj {
    int& i;
    DtorTestObj(int& i) : i(i) { }
    DtorTestObj(DtorTestObj&&) = default;
    DtorTestObj(const DtorTestObj&) = default;
    ~DtorTestObj() { i++; }
};

#include <exception>
#include <cstdlib>

inline void test_setup() {
    std::set_terminate([]() { std::quick_exit(254); });
}
