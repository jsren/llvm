
int foo(bool b)
{
    return b + 4;
}

struct Example
{
    long bar(bool b) {
        return b + 5;
    }
    operator bool() {
        return false;
    }
    static int kek() {
        return 0;
    }
};

void(*ptr)();

int main()
{
    ptr();
    foo(true);
    Example e;
    e.bar(true);
    (&e)->bar(false);
    e.operator bool();
    Example::kek();
}
