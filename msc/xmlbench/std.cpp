/* std.cpp - (c) 2018 James Renwick */
#include "std.hpp"
#include <string.h>

static string out_of_memory_what{};
static string* exception_what{};
static string* index_out_of_range_what{};
static string* file_not_found_what{};

[[gnu::weak]] alignas(1)
    char __typeid_for_file_not_found = 0;
[[gnu::weak]] alignas(1)
    char __typeid_for_index_out_of_range = 0;
[[gnu::weak]] alignas(1)
    char __typeid_for_out_of_memory = 0;
[[gnu::weak]] alignas(1)
    char __typeid_for_exception = 0;

[[gnu::weak]] alignas(alignof(char*))
char* __typeid_bases_for_exception[] = {
    nullptr
};
[[gnu::weak]] alignas(alignof(char*))
char* __typeid_bases_for_out_of_memory[] = {
    &__typeid_for_exception,
    nullptr
};
[[gnu::weak]] alignas(alignof(char*))
char* __typeid_bases_for_index_out_of_range[] = {
    &__typeid_for_exception,
    nullptr
};
[[gnu::weak]] alignas(alignof(char*))
char* __typeid_bases_for_file_not_found[] = {
    &__typeid_for_exception,
    nullptr
};


const string& exception::what() const noexcept
{
    if (exception_what == nullptr) {
        exception_what = stdnew<string>("an exception occurred");
    }
    return *exception_what;
}

const string& index_out_of_range::what() const noexcept
{
    if (index_out_of_range_what == nullptr) {
        index_out_of_range_what = stdnew<string>("the given index was out of range");
    }
    return *index_out_of_range_what;
}

const string& file_not_found::what() const noexcept
{
    if (file_not_found_what == nullptr) {
        file_not_found_what = stdnew<string>("the file could not be opened");
    }
    return *file_not_found_what;
}

const string& out_of_memory::what() const noexcept
{
    return out_of_memory_what;
}

string::string(const char* cstr) throws
    : size(::strlen(cstr)),
      data(reinterpret_cast<char*>(::malloc(size + 1)))
{
    if (data == nullptr) throw out_of_memory();
    ::memcpy(data, cstr, size);
    data[size] = '\0';
}

string::string(char c, size_type size) throws
    : size(size), data(reinterpret_cast<char*>(::malloc(size + 1)))
{
    if (data == nullptr) throw out_of_memory();
    ::memset(data, c, size);
    data[size] = '\0';
}

string::string(const char* cstr, size_type size) throws
    : size(size), data(reinterpret_cast<char*>(::malloc(size + 1)))
{
    if (data == nullptr) throw out_of_memory();
    ::memcpy(data, cstr, size);
    data[size] = '\0';
}

string::string(const string& other) throws
    : size(other.size), data(reinterpret_cast<char*>(::malloc(size + 1)))
{
    if (data == nullptr) throw out_of_memory();
    ::memcpy(data, other.data, size + 1);
}

string::string(string&& other) noexcept
    : size(other.size), data(other.data)
{
    other.data = nullptr;
    other.size = 0;
}

string::~string()
{
    if (data != nullptr) {
        ::free(data);
    }
}

string& string::operator=(string&& other) noexcept {
    auto tsize = size;
    auto tdata = data;
    size = other.size;
    data = other.data;
    other.size = tsize;
    other.data = tdata;
    return *this;
}

char& string::at(size_type index) throws
{
    if (index >= size) throw index_out_of_range(index);
    else return data[index];
}
const char& string::at(size_type index) const throws
{
    if (index >= size) throw index_out_of_range(index);
    else return data[index];
}

file_stream::file_stream(const string& filepath) throws
{
    if ((handle = ::fopen(filepath.c_str(), "r")) == nullptr) {
        throw file_not_found();
    }
}

file_stream::file_stream(file_stream&& other) noexcept
    : offset(other.offset), handle(other.handle), eof(other.eof)
{
    other.handle = nullptr;
}


char file_stream::read() noexcept
{
    int c = ::fgetc(handle);
    if (c == EOF) eof = true;
    else { offset++; }
    return c;
}

char file_stream::peek() noexcept
{
    int c = ::fgetc(handle);
    ::ungetc(c, handle);
    return c;
}

void file_stream::seek(size_type offset) throws {
    if (::fseek(handle, offset, SEEK_SET) != 0) {
        throw file_not_found();
    }
    this->offset = offset;
}

bool file_stream::is_end() const noexcept {
    return eof;
}

size_type file_stream::position() const noexcept {
    return offset;
}

file_stream::~file_stream() {
    if (handle != nullptr) ::fclose(handle);
}

stream_base::~stream_base() {
}
