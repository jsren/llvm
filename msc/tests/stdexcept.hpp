#pragma once
#include <type_traits>

extern "C" {
    alignas(64)
    static unsigned char __exception_obj_buffer[64];
}

struct __exception_t {
    void* type;
    decltype(sizeof(int)) size;
    // If the original ctor is not 'throws', we will emit and assign a thunk instead
    // TODO: support VTT pointer
    void(*ctor)(void*, __exception_t*, void*); 
    void(*dtor)(void*);
};
static __exception_t __type_dummy;

static_assert(std::is_trivially_copyable<__exception_t>::value,"");
static_assert(std::is_trivially_destructible<__exception_t>::value,"");

extern "C" {
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

    struct [[gnu::packed]] __tinfo_entry_t {
        void* base;
        void* derived;
    };

    [[gnu::section(".tinfo"), gnu::used]]
    alignas(sizeof(void*))
    static __tinfo_entry_t __tinfo[] = {
        __tinfo_entry_t{&__typeid_for_BaseObj, &__typeid_for_SuperObj}
    };

    extern const char __tinfo_start;
    extern const char __tinfo_end;

    [[gnu::noinline]]
    static bool __type_is_not_base(void* base, void* super) noexcept {
        for (auto* e = reinterpret_cast<const __tinfo_entry_t*>(&__tinfo_start); e < (void*)&__tinfo_end; e++) {
            if (e->base == base && e->derived == super) return false;
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
    ~DtorThrowsObj() { throw 0; }
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
