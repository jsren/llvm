/* xmlbench.cpp - (c) 2018 James Renwick */
#pragma once
#include "std.hpp"

struct XMLException : exception
{
    virtual const string& what() const noexcept override = 0;
};

struct MultipleRootException final : XMLException
{
    virtual const string& what() const noexcept override;
};

struct BadSyntaxException final : XMLException
{
    size_type column;
    size_type row;
    inline BadSyntaxException(size_type column, size_type row)
        : column(column), row(row) { }
    inline BadSyntaxException(BadSyntaxException&& e) throws
        : column(e.column), row(e.row) { }
    inline BadSyntaxException(const BadSyntaxException& e) throws
        : column(e.column), row(e.row) { }
    virtual const string& what() const noexcept override;
};

namespace xmlb
{
    class XMLAttribute
    {
    public:
        string name;
        string value;

        XMLAttribute() noexcept = default;
        XMLAttribute(string name, string value) noexcept;
        XMLAttribute(const XMLAttribute& other) throws;
        XMLAttribute(XMLAttribute&& other) noexcept;
        XMLAttribute& operator =(XMLAttribute&&) noexcept;
    };

    class XMLNode
    {
    public:
        string tagName;
        list<XMLNode> children;
        list<XMLAttribute> attributes;

        XMLNode() noexcept = default;
        XMLNode(string tagName,
                list<XMLAttribute> attributes,
                list<XMLNode> children) noexcept;

        XMLNode(const XMLNode& other) throws;
        XMLNode(XMLNode&&) noexcept = default;
        XMLNode& operator=(XMLNode&&) noexcept = default;
    };

    XMLNode loadFromFile(file_stream& fs) throws;
}
