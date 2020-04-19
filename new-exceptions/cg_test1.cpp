struct __exception_t
{
    void* type;
    bool success;
    void(*dtor)(void*);
};

bool test2()
{
    return __exception.success;
}

bool test()
{
    return test2();
}

int main() noexcept
{
    __exception_t __exception;
    __exception.success = true;

    auto res = test();
    return res;
}
