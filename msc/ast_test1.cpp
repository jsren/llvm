
extern "C" {
    struct __exception_t {
        void* type;
        bool threw;
        void(*dtor)(void*, __exception_t*);
    };
}

extern "C" {
    char __typeid_for_int = 0;
    char __typeid_for_Payload = 0;
}

struct Payload
{
    int i = 0x42;

    Payload() throws// = delete; {
    {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
    Payload(const Payload& o) throws {
        i = o.i;
    };
    Payload& operator =(const Payload&) = delete;

    ~Payload() throws {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
};

Payload test() throws
{
    __builtin_throw(Payload());
    //auto P = Payload();
    //P.i = 1;
    //return P;
}

int main()
{
    __exception_t __dummy;

    //try
    __builtin_try();
    {
        __builtin_try();
        {
            Payload p = test();
            // TODO: use p
        }
        __builtin_catch();
        {
            __builtin_throw(0);
            return 2;
        }
        __builtin_catch_end();
    }
    __builtin_catch();
    {
        __builtin_throw(0);
        return 1;
    }
    __builtin_catch_end();
    return 0;
}
