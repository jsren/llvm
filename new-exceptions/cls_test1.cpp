
bool callee(__exception* e) {
    // ...
    if (error) {
        e->success = false;
        return __builtin_empty_return();
    }
    // ...
    return false;
}

void caller(__exception* e) {
    {
    _try:
        bool b = callee(e);
        if (!e->success) goto _catch;
        // Otherwise use b
    }
    {
    _catch:

    }
}

bool callee() {
    // ...
    if (error) {
        throw;
    }
    // ...
    return false;
}

void caller() {
    try {
        bool b = callee();
    }
    catch (...) {
        //...
    }
}


extern "C" char __typeid_for_int = 0;


alignas(alignof(MAX_EXCEPTION_OBJ_ALIGN))
    unsigned char __exception_buffer[MAX_EXCEPTION_OBJ_SIZE];


throw T(args...);

new (__exception_buffer) T(args...);

catch (const T& t) {
    // Use t
}

{
_catch:
    T __exception_obj{
        std::move(reinterpret_cast<T*>(__exception_buffer))};
    const T& t = __exception_obj;
}
