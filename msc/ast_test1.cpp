
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
    //__exception.success = false;
    __builtin_throw(Payload());
    return __builtin_empty_return();
    auto P = Payload();
    P.i = 1;
    return P;
}

int main()
{
    __exception_t __dummy;

    //try
    {
        Payload p = test();
        //if (!__exception.success) goto handler;
    }
handler:
    {
        
    }
}
