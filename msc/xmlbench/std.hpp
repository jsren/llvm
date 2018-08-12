/* std.hpp - (c) 2018 James Renwick */
#pragma once
#include <stdexcept.hpp>
#include <stdlib.h>
#include <stdio.h>

// Used for IDE support
//#if !ZCEXCEPTIONS
//#define throws noexcept(false)
//#endif

using size_type = decltype(sizeof(int));

//extern unsigned char* stdheap_start;

class string;

class exception
{
public:
    virtual ~exception() = default;
    virtual const string& what() const noexcept;
};

class out_of_memory final : public exception {
public:
    inline out_of_memory() noexcept { }
    inline out_of_memory(const out_of_memory&) throws { }
    inline out_of_memory(out_of_memory&&) throws { }
    const string& what() const noexcept override;
};

class index_out_of_range final : public exception {
public:
    const size_type index{};
    inline index_out_of_range(size_type index) : index(index) { }
    index_out_of_range(const index_out_of_range&) throws { };
    index_out_of_range(index_out_of_range&&) throws { };
    const string& what() const noexcept override;
};

class file_not_found final : public exception {
public:
    inline file_not_found() noexcept { }
    inline file_not_found(const file_not_found&) throws { }
    inline file_not_found(file_not_found&&) throws { }
    const string& what() const noexcept override;
};

template<typename T, typename ...Args>
T* stdnew(Args&&... args) throws
{
    // Quick and dirty alignment
    /*auto offset = alignof(T) - (static_cast<size_type>(heap_start) % alignof(T));
    if (offset == alignof(T)) offset = 0;
    unsigned char* end = heap_start + offset + sizeof(T);*/

    auto obj = reinterpret_cast<T*>(::aligned_alloc(alignof(T), sizeof(T)));

    // Check for heap exhaustion
    /*if (end > heap_start + sizeof(heap)) {*/
    if (obj == nullptr) {
        throw out_of_memory();
    }

    //unsigned char* obj = heap_start + offset;
    //heap_start = end;
    // Initialise object
    return new (obj) T(static_cast<Args&&>(args)...);
}

template<typename T>
void stddelete(T* object) throws
{
    // Call object destructor
    object.T::~T();
    // Deallocate
    ::free(object);
}

class string
{
    size_type size{};
    char* data{};

public:
    string() noexcept = default;
    string(char c, size_type count) throws;
    string(const char* cstr) throws;
    string(const char* cstr, size_type size) throws;
    string(const string& str) throws;
    string(string&& str) noexcept;
    ~string();

    string& operator=(string&&) noexcept;

    char& at(size_type index) throws;
    const char& at(size_type index) const throws;

    inline char* c_str() noexcept {
        return data;
    }
    inline const char* c_str() const noexcept {
        return data;
    }
    inline size_type length() const noexcept {
        return size;
    }
};

template<typename T>
class list
{
    size_type _size{};
    T* _data{};

public:
    list() noexcept = default;
    list(size_type size) throws
        // TODO: this is not a safe way of allocating arrays
        : _size(size),
          _data(reinterpret_cast<T*>(::aligned_alloc(alignof(T), sizeof(T)*size)))
    {
        if (_data == nullptr) throw out_of_memory();
        for (size_type i = 0; i < size; i++) {
            new (&_data[i]) T();
        }        
    }
    // TODO: make throws dependent on type -> throws(T{*((T*)0)})
    list(const list& other) throws
        // TODO: this is not a safe way of allocating arrays
        : _size(other._size),
          _data(reinterpret_cast<T*>(::aligned_alloc(alignof(T), sizeof(T)*other._size)))
    {
        for (size_type i = 0; i < _size; i++) {
            new (&_data[i]) T(other._data[i]);
        }
    }
    list(list&& other) noexcept
        : _size(other._size), _data(other._data) 
    {
        other._size = 0;
        other._data = nullptr;
    }

    list& operator=(list&& other) noexcept {
        auto tsize = _size;
        auto tdata = _data;
        _size = other._size;
        other._size = tsize;
        _data = other._data;
        other._data = tdata;
        return *this;
    }

    ~list() {
        if (_data != nullptr) ::free(_data);
    }

    T* data() noexcept {
        return _data;
    }
    const T* data() const noexcept {
        return _data;
    }

    T& operator[](size_type index) noexcept {
        return _data[index];
    }
    const T& operator[](size_type index) const noexcept {
        return _data[index];
    }

    T& at(size_type index) throws {
        if (index >= _size) throw index_out_of_range(index);
        return _data[index];
    }
    const T& at(size_type index) const throws {
        if (index >= _size) throw index_out_of_range(index);
        return _data[index];
    }
    size_type size() const noexcept {
        return _size;
    }
};

class stream_base
{
public:
    virtual ~stream_base();

    virtual char read() noexcept = 0;
    virtual void seek(size_type offset) throws = 0;
    virtual bool is_end() const noexcept = 0;
    virtual size_type position() const noexcept = 0;
};


class file_stream : public stream_base
{
    size_type offset{};
    ::FILE* handle{};
    bool eof{};

public:
    file_stream(const string& filepath) throws;
    file_stream(file_stream&& other) noexcept;
    ~file_stream();
    char read() noexcept override;
    char peek() noexcept;
    void seek(size_type offset) throws override;
    bool is_end() const noexcept override;
    size_type position() const noexcept override;
};
