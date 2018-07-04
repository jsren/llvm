struct __exception_t
{
    void* type;
    bool success;
    void(*dtor)(void*);
};


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
    auto P = Payload();
    P.i = 1;
    return P;
    //return __builtin_return_empty();
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
