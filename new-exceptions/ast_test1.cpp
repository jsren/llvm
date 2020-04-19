#include "stdexcept.hpp"
#include <cstdio>

extern "C" {
    [[gnu::weak]]
        char __typeid_for_int = 0;
    [[gnu::weak]]
        char __typeid_for_Payload = 0;
}

struct Payload
{
    int i = 42;

    Payload(int o) : i(o) {
        std::printf("Payload constructed\n");
        //std::cout << "Payload constructed\n";
        //*((volatile int*)0x666) = 0xEE;
        /*throw 3;*/
    }
    Payload(const Payload& o) : i(o.i) {
        //*((volatile int*)0x777) = 0xC0;
        std::printf("Payload copy constructed\n");
        //std::cout << "Payload copy constructed with " << i << '\n';
    };
    Payload(Payload&& o) throws : i(o.i) {
        std::printf("Payload move constructed\n");
        throw 54;
        //*((volatile int*)0x777) = 0xC0;
        //std::cout << "Payload move constructed\n";
    }
    Payload& operator =(const Payload&) = delete;
    ~Payload() {
        //*((volatile int*)0x888) = 0xFF;
        std::printf("Payload destroyed.\n");
        //std::cout << "Dead Payload contained " << i << '\n';
    }
};

Payload test() throws;

/*int main()
{
    try {
        try {
            Payload i(99);
            int j = [&]() throws {
                throw 1;
                return i.i;
            }();
            return j;
            //Payload p = test();
        }
        catch (Payload p) {
            return 7;
        }
        catch(...) {
            throw 0;
            //return 0;
        }
    }
    catch(Payload p) {
        return 0;
    }
    catch(int i) {
        return 5;
    }
    catch(...) {
        throw 13;
    }
    return 0;
}*/

int other() throws
{
    try
    {
        throw Payload(77);
        //throw 0;
        //Payload(2);
    }
    catch (Payload p)
    {
        //Payload pp = p;
        return p.i;
    }
    /*catch (...)
    {
        return 2;
    }*/
    return 0;
}

int main()
{
    //throw 0;
    return other();
    /*try { 
        return other();
    }
    catch (int i) {
        return i;
    }
    return 0;*/
}

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