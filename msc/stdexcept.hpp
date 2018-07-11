struct __exception_t {
    void* type;
    bool threw;
    void(*dtor)(void*);
};
static __exception_t __type_dummy;

extern "C" {
    unsigned char __exception_obj_buffer[64];
}

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
}
