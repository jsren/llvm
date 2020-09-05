#pragma once
#include <string>
#include <stdexcept>
#include <fstream>
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_os_ostream.h"
#include "clang/AST/Type.h"
#include "clang/AST/Stmt.h"

namespace Debug
{
    namespace _detail
    {
        struct file_container
        {
        private:
            static std::fstream open_file()
            {
                return std::fstream("clang-debug-log.asm", std::ios_base::out);
            }
        public:
            static std::ostream& get_file()
            {
                static auto file = open_file();
                return file;
            };
        };
    }

    inline static void Print(std::ostream& stream, bool value) noexcept
    {
        stream << (value ? "true" : "false") << std::endl;
    }

    inline static void Print(std::ostream& stream, char value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, unsigned char value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, short value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, unsigned short value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, int value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, unsigned int value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, long value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, unsigned long value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, long long value) noexcept {
        stream << value << std::endl;
    } inline static void Print(std::ostream& stream, unsigned long long value) noexcept {
        stream << value << std::endl;
    }

    inline static void Print(std::ostream& stream, const char* string) noexcept
    {
        stream << string << std::endl;
    }
    inline static void Print(std::ostream& stream, const std::string& string) noexcept
    {
        stream << string << std::endl;
    }

    inline static void Print(std::ostream& stream, const llvm::Value* value)
    {
        llvm::raw_os_ostream llvm_stream(stream);
        value->print(llvm_stream, true);
        stream << std::endl;
    }
    inline static void Print(std::ostream& stream, const clang::Stmt* value)
    {
        llvm::raw_os_ostream llvm_stream(stream);
        value->dump(llvm_stream);
    }
    inline static void Print(std::ostream& stream, const clang::QualType* value)
    {
        llvm::raw_os_ostream llvm_stream(stream);
        value->dump(llvm_stream);
        stream << std::endl;
    }


    template<typename ...Ts>
    void Log(const std::string& event_type, const std::string& target, Ts&&... params)
    {
        auto& file = _detail::file_container::get_file();
        file << "BEGIN " << event_type << " " << target << " {" << std::endl;

        (Print(file, params), ...);

        file << "} END " << event_type << " " << target << std::endl << std::endl << std::endl;
    }
}
