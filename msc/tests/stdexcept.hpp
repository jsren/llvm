/* stdexcept.hpp - (c) 2018 James Renwick */
#pragma once
#include <type_traits>
#include <cstdlib>
#include <new>

extern "C" {
    alignas(64)
    static /*thread_local*/ unsigned char __exception_obj_buffer[1024];

    static unsigned char* __exception_obj_ptr = __exception_obj_buffer;
}

inline unsigned char* __cxa_allocate_exception_obj(
    decltype(sizeof(int)) size, char alignment) noexcept
{
    if (__exception_obj_ptr + size > __exception_obj_buffer + sizeof(__exception_obj_buffer)) {
        return new /*((std::align_val_t)alignment)*/ unsigned char[size]();
    }
    // TODO: Handle alignment correctly - this doesn't impact on WCET
    auto out = __exception_obj_ptr;
    __exception_obj_ptr += size;
    return out;
}

inline void __cxa_free_exception_obj(unsigned char* ptr, char alignment) noexcept {
    if (ptr < __exception_obj_buffer || ptr > __exception_obj_buffer + sizeof(__exception_obj_buffer)) {
        ::operator delete[](ptr/*, (std::align_val_t)alignment*/);
    }
}

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
    // TODO: support VTT pointer
    void(*ctor)(void*, __exception_t*, void*); 
    void(*dtor)(void*);
    bool active;
    bool ptr;
    // Alignment of exception object
    char alignment;
};
// Force Clang to emit ctor/dtor definitions
// TODO: Automatically mark as 'used', removing need for this variable
static __exception_t __type_dummy;

static_assert(std::is_trivially_copyable<__exception_t>::value,"");
static_assert(std::is_trivially_destructible<__exception_t>::value,"");


class __exception_obj_base {
    __exception_t exception{};

protected:
    constexpr __exception_obj_base(unsigned char* c, __exception_t e) noexcept
        : exception(e) { exception.buffer = c; }
public:
    __exception_obj_base() = default;
    __exception_obj_base(const __exception_obj_base&) = default;
    virtual ~__exception_obj_base() = default;
};
// Force Clang to emit ctor/dtor definitions
// TODO: Automatically mark as 'used', removing need for this variable
static __exception_obj_base __type_dummy2;


extern "C" {
    [[gnu::weak]] alignas(1)
        char __typeid_for_nullptr_t = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_int = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_1int = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_bool = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_EmptyObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_TrivialObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_NoTrivialObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_NoTrivialDtorObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_NoTrivialMoveObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_NoMoveObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_CtorThrowableObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_MoveThrowableObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_CtorThrowsObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_MoveThrowsObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_DtorThrowsObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_BaseObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_1BaseObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_SuperObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_1SuperObj = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_DtorTestObj = 0;

    [[gnu::weak]] alignas(alignof(char*))
    char* __typeid_empty_bases[1] = { nullptr };

    // Alternative form
    [[gnu::weak]] alignas(alignof(char*))
    char* __typeid_bases_for_SuperObj[] = {
        &__typeid_for_BaseObj,
        nullptr
    };

    [[gnu::noinline]]
    static bool __type_is_not_base(void* type, const char** super_bases) noexcept {
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
struct DtorTestObj {
    int& i;
    DtorTestObj(int& i) : i(i) { }
    DtorTestObj(DtorTestObj&&) = default;
    DtorTestObj(const DtorTestObj&) = default;
    ~DtorTestObj() { i++; }
};

#include <exception>
#include <cstdlib>

static void test_setup() {
    std::set_terminate([]() { std::quick_exit(254); });
}
