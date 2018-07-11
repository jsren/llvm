#include "stdexcept.hpp"
#include <iostream>

extern "C" {
    char __typeid_for_int = 0;
    char __typeid_for_Payload = 0;
}

struct Payload
{
    int i = 42;

    Payload(int o) throws : i(o) { /*throw 3;*/ }
    Payload(const Payload& o) throws : i(o.i) { };
    Payload& operator =(const Payload&) = delete;
    ~Payload() {
        std::cout << "Payload contained " << i << '\n';
    }
};

Payload test() throws
{
    if (true) {
        throw Payload(52);
    }
    else return Payload(53);
}


int main()
{
    try {
        try {
            Payload i(2);
            Payload p = test();
        }
        catch (Payload p) {
            return 7;
        }
        catch(...) {
            throw 0;
        }
    }
    catch(Payload p) {
        throw 0;
    }
    catch(int i) {
        return 5;
    }
    catch(...) {
        throw 0;
    }
    return 0;
}

/*int other() throws
{
    try
    {
        //throw Payload(2);
        //throw 0;
        Payload(2);
    }
    catch (Payload p)
    {
        return 1;
    }
    catch (...)
    {
        return 2;
    }
    return 0;
}

int main()
{
    try { 
        other();
    }
    catch (int i) {
        return 4;
    }
    return 0;
}*/

/*int main()
{
    TRY {
        TRY {
            Payload p = test();
        }
        CATCH({
            THROW(0);
        })
    }
    CATCH(
    catch_payload:
    {
        if (std::exception_info().get_typeid() != &__typeid_for_Payload) goto catch_int;
        THROW(0);
        goto catch_end;
    }
    catch_int:
    {
        if (std::exception_info().get_typeid() != &__typeid_for_int) goto catch_all;
        return 5;
        goto catch_end;
    }
    catch_all:
    {
        THROW(0);
        goto catch_end;
    }
    catch_end:
    __builtin_get_exception();
    )
    return 0;
}
*/