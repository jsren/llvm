struct __exception_t
{
    void* type;
    bool success;
    void(*dtor)(void*);
};

extern "C" {
    char __typeid_for_int = 0;
}

struct Payload
{
    int i = 0x42;

    Payload()// = delete; {
    {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
    Payload(const Payload& o) {
        i = o.i;
    };
    Payload& operator =(const Payload&) = delete;

    ~Payload() {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
};

Payload test()
{
    //__exception.success = false;
    __builtin_throw(1);
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
