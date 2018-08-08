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
    // Alignment of exception object
    char alignment;
    // If the original ctor is not 'throws', we will emit and assign a thunk instead
    // TODO: support VTT pointer
    void(*ctor)(void*, __exception_t*, void*); 
    void(*dtor)(void*);
    bool active;
    bool ptr;
};
static __exception_t __type_dummy;

static_assert(std::is_trivially_copyable<__exception_t>::value,"");
static_assert(std::is_trivially_destructible<__exception_t>::value,"");

extern "C" {
    [[gnu::weak]] alignas(1)
        char __typeid_for_nullptr_t = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_int = 0;
    [[gnu::weak]] alignas(1)
        char __typeid_for_int___ptr = 0;
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
        char __typeid_for_SuperObj = 0;
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

    /*struct [[gnu::packed]] __tinfo_entry_t {
        void* base;
        void* derived;
    };

    [[gnu::section(".tinfo"), gnu::used]]
    alignas(sizeof(void*))
    static __tinfo_entry_t __tinfo[] = {
        __tinfo_entry_t{&__typeid_for_BaseObj, &__typeid_for_SuperObj}
    };*/

    /*extern const char __tinfo_start;
    extern const char __tinfo_end;*/

    /*[[gnu::noinline]]
    static bool __type_is_not_base(void* base, void* super) noexcept {
        for (auto* e = reinterpret_cast<const __tinfo_entry_t*>(&__tinfo_start); e < (void*)&__tinfo_end; e++) {
            if (e->base == base && e->derived == super) return false;
        }
        return true;
    }*/
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
};
struct DtorTestObj {
    int& i;
    DtorTestObj(int& i) : i(i) { }
    DtorTestObj(DtorTestObj&&) = default;
    DtorTestObj(const DtorTestObj&) = default;
    ~DtorTestObj() { i++; }
};

namespace std
{
    using type_id = void*;

    class exception_info
    {
        void(*dtor)(void*);
        type_id type;

    public:
        exception_info() throws {
            const __exception_t* e = __builtin_get_exception();
            dtor = e->dtor;
            type = e->type;
        }

    public:
        exception_info(const exception_info&) = default;

        inline type_id get_typeid() const {
            return type;
        }

        inline void destroy_obj() {
            if (dtor) {
                dtor(__exception_obj_buffer);
            }
        }
    };
}

#include <exception>
#include <cstdlib>

static void test_setup() {
    std::set_terminate([]() { std::quick_exit(254); });
}
