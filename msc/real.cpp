#include <iostream>

struct Death {
    Death() = default;
    Death(int i) {
        throw 3;
    }
};

struct Payload
{
    int i = 42;
    Death d;

    Payload(int o) try : i(o), d(o) {
        std::cout << "Not Caught!\n";
    }
    catch(...) {
        std::cout << "Caught!\n";
    }
    Payload(const Payload& o) : i(o.i) { };
    Payload& operator =(const Payload&) = delete;
    ~Payload() {
        std::cout << "Payload contained " << i << '\n';
    }
};

int main()
{
    try {
    Payload(2);
    }
    catch (int) {
        return 1;
    }
    return 0;
}
