struct __exception_t
{
    void* type;
    bool success;
    void(*dtor)(void*);
};


struct Payload
{
    int i = 0x42;

    Payload() noexcept// = delete; {
    {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
    Payload(const Payload&) noexcept {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    };
    Payload& operator =(const Payload&) noexcept = delete;

    ~Payload() noexcept {
        //*(volatile int*)(0xC0FFEEBA) = 0xBE;
    }
};


Payload test()
{
    //__exception.success = false;
    return Payload();
    //return __builtin_return_empty();
}

int main() noexcept
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
