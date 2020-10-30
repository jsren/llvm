/* rethrow.hpp - (c) 2018 James Renwick */
#pragma once
#include <memory>
#include "common.hpp"

namespace std
{
    template<typename Alloc = allocator<unsigned char>>
    class exception_obj final : public __exception_obj_base {
    public:
        using allocator = Alloc;
        friend exception_obj<Alloc>
            __make_exception_obj(unsigned char*) throws;

    private:
        allocator alloc;
        bool stacklocal{};

        constexpr exception_obj(const __exception_obj_base* base, allocator alloc) noexcept
                : __exception_obj_base(*base), alloc(alloc), stacklocal(true) { }

    public:
        exception_obj(exception_obj&& other) throws
            : exception_obj(std::move(other), other.alloc) { }

        template<typename A>
        exception_obj(exception_obj<A>&& other) throws
            : exception_obj(std::move(other), allocator{}) { }

        template<typename A>
        exception_obj(exception_obj<A>&& other, allocator alloc) throws
            : __exception_obj_base(other), alloc(alloc)
        {
            // If stack-allocated within catch block, re-alloc using allocator
            // Or if allocators differ, move between allocators
            if (alloc != other.alloc || other.stacklocal) {
                // TODO: adjust alignment
                alloc.allocate(other.exception.size + other.exception.alignment);
                exception.ctor(data, const_cast<__exception_t*>(__builtin_get_exception()), other.data);
                exception.dtor(other.data);

                if (!other.stacklocal) {
                    other.alloc.deallocate(other.data,
                        other.exception.size + other.exception.alignment);
                }
            }
            // Otherwise just pass memory around
            else other.data = nullptr;
        }

        ~exception_obj() noexcept
        {
            if (!stacklocal && data != nullptr) {
                exception.dtor(data);
                try { alloc.deallocate(data, exception.size + exception.alignment); }
                catch (...) { }
            }
        }
    };

    static exception_obj<> __make_exception_obj() throws {
        return exception_obj<>(__builtin_get_exception_obj(), allocator<unsigned char>());
    }
}
