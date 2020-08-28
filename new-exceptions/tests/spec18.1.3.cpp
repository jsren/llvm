// expected: 21
/*
Throwing an exception copy-initializes (11.6, 15.8) a temporary object, called the exception object. An lvalue
denoting the temporary is used to initialize the variable declared in the matching handler (18.3). If the type
of the exception object would be an incomplete type or a pointer to an incomplete type other than cv void
the program is ill-formed.

NB There are three copies here: once into the buffer, once out of the buffer into hidden-variable
(for original rethrowing) and once more into the catch variable.
*/
#include "stdexcept.hpp"

[[gnu::weak]] alignas(1)
    char __typeid_for_Test = 0;

struct Test {
    int& value;
    Test(int& value) : value(value) { }
    Test(const Test& other) : value(other.value) {
        value += 7;
    }
};

int main()
{
    test_setup();
    int out = 0;
    try {
        Test t(out);
        throw t;
    }
    catch (Test t) {
        return out;
    }
}
