struct __exception_t {
    void* type;
    bool threw;
    void(*dtor)(void*, __exception_t*);
};

int main()
{
    goto label;
    {
        *((volatile int*)(0x0)) = 0;
        return 0;
    }
    label:
    {
        return 1;
    }
    return 2;
}
