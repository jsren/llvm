char __TypeInfoFor_int = 0;

struct __exception_t
{
    void* type;
    bool success;
    void(*dtor)(void*);
} e;

void test()
{
    __builtin_throw();
}

int main()
{
    test();
    return 0;    
}
