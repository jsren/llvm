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

#define TRY __builtin_try();
#define CATCH(x) __builtin_catch(); {x} __builtin_catch_end();
#define THROW(x) __builtin_throw(x)

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

    inline bool __type_is_base(type_id base, type_id super) noexcept {

    }
}
