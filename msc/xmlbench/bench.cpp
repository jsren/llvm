#include "xmlbench.hpp"
#include <string.h>

extern "C" {
[[gnu::weak, gnu::section(".typeids")]] alignas(1)
    extern const char __typeid_for_exception = 0;
[[gnu::weak, gnu::section(".typeids")]] alignas(1)
    extern const char __typeid_for_XMLException = 0;
[[gnu::weak, gnu::section(".typeids")]] alignas(1)
    extern const char __typeid_for_MultipleRootException = 0;
[[gnu::weak, gnu::section(".typeids")]] alignas(1)
    extern const char __typeid_for_BadSyntaxException = 0;

[[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
extern const char* __typeid_bases_for_XMLException[] = {
    &__typeid_for_exception,
    nullptr
};
[[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
extern const char* __typeid_bases_for_MultipleRootException[] = {
    &__typeid_for_exception,
    &__typeid_for_XMLException,
    nullptr
};
[[gnu::weak, gnu::section(".typeid-bases")]] alignas(alignof(char*))
extern const char* __typeid_bases_for_BadSyntaxException[] = {
    &__typeid_for_exception,
    &__typeid_for_BadSyntaxException,
    nullptr
};
}

namespace xmlb
{
    XMLAttribute::XMLAttribute(string name, string value) noexcept
        : name(static_cast<string&&>(name)),
            value(static_cast<string&&>(value))
    { }
    XMLAttribute::XMLAttribute(XMLAttribute&& other) noexcept
        : name(static_cast<string&&>(other.name)),
            value(static_cast<string&&>(other.value))
    { }
    XMLAttribute::XMLAttribute(const XMLAttribute& other) throws
        : name(other.name), value(other.value)
    { }

    XMLAttribute& XMLAttribute::operator =(XMLAttribute&& other) noexcept
    {
        name = static_cast<string&&>(other.name);
        value = static_cast<string&&>(other.value);
        return *this;
    }

    XMLNode::XMLNode(string tagName,
                     list<XMLAttribute> attributes,
                     list<XMLNode> children) noexcept
        : tagName(static_cast<string&&>(tagName)),
          children(static_cast<list<XMLNode>&&>(children)),
          attributes(static_cast<list<XMLAttribute>&&>(attributes))
    { }
    /*XMLNode::XMLNode(XMLNode&& other) noexcept
        : tagName(static_cast<string&&>(other.tagName)),
          children(static_cast<list<XMLNode>&&>(other.children)),
          attributes(static_cast<list<XMLAttribute>&&>(other.attributes))
    { }*/
    XMLNode::XMLNode(const XMLNode& other) throws
        : tagName(other.tagName),
          children(other.children),
          attributes(other.attributes)
    { }

    /*XMLNode& XMLNode::operator =(XMLNode&& other) noexcept
    {
        tagName = static_cast<string&&>(other.tagName);
        children = static_cast<list<XMLNode>&&>(other.children);
        attributes = static_cast<list<XMLAttribute>&&>(other.attributes);
    }*/

    static void assertEx(
        bool condition, size_type column, size_type row) throws
    {
        if (!condition) throw BadSyntaxException(column, row);
    }

    // This is quick and dirty
    static bool isNameChar(char c) noexcept {
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
              || ('0' <= c && c <= '9');
    }

    static void skipWhitespace(
        file_stream& fs, size_type& col, size_type& row) noexcept
    {
        while (true)
        {
            char c = fs.peek();
            if (c == '\n') {
                col = 0;
                row++;
            }
            else if (c == '\r') {
            }
            else if (c == ' ' || c == '\t') {
                col++;
            }
            else return;
            fs.read();
        }
    }

    static string parseName(file_stream& fs) throws
    {
        auto start = fs.position();
        while (isNameChar(fs.read())) { }
        auto length = (fs.position() - 1) - start;
        fs.seek(start);

        string name{'\0', length};
        for (size_t i = 0; i < length; i++) {
            name.c_str()[i] = fs.read();
        }
        return name;
    }

    static string parseValue(
        file_stream& fs, size_type& col, size_type& row) throws
    {
        assertEx(fs.read() == '"', col, row);
        auto start = fs.position();
        while (fs.read() != '"') { }
        auto length = (fs.position() - 1) - start;
        fs.seek(start);

        string name{'\0', length};
        for (size_t i = 0; i < length; i++) {
            name.c_str()[i] = fs.read();
        }
        assertEx(fs.read() == '"', col, row);
        return name;
    }

    static bool tryParseCloseTag(
        file_stream& fs, string& name, size_type& col, size_type& row) throws
    {
        auto start = fs.position();
        assertEx(fs.read() == '<', col++, row);
        skipWhitespace(fs, col, row);

        if (fs.read() != '/') {
            fs.seek(start);
            return false;
        }

        string tagName = parseName(fs);
        assertEx(tagName.length() != 0, col, row);
        assertEx(::strcmp(tagName.c_str(), name.c_str()) == 0, col, row);
        col += tagName.length();

        skipWhitespace(fs, col, row);
        assertEx(fs.read() == '>', col++, row);
        return true;
    }

    static XMLNode parseNode(
        file_stream& fs, size_type& col, size_type& row) throws
    {
        assertEx(fs.read() == '<', col++, row);
        skipWhitespace(fs, col, row);
    
        string tagName = parseName(fs);
        assertEx(tagName.length() != 0, col, row);
        col += tagName.length();

        constexpr size_type maxAtts = 5;
        string attNames[maxAtts]{};
        string attValues[maxAtts]{};

        size_type attCount = 0;
        while (true) {
            assertEx(attCount < maxAtts, col, row);
            skipWhitespace(fs, col, row);
            string name = parseName(fs);
            if (name.length() == 0) {
                break;
            }
            col += name.length();
            attNames[attCount] = static_cast<string&&>(name);
            skipWhitespace(fs, col, row);
            
            assertEx(fs.read() == '=', col++, row);
            skipWhitespace(fs, col, row);

            attValues[attCount++] = static_cast<string&&>(parseValue(fs, col, row));
        }

        assertEx(fs.read() == '>', col++, row);
        skipWhitespace(fs, col, row);

        constexpr size_type maxChildren = 5;
        XMLNode children[maxChildren]{};
        size_type childCount = 0;
        while (!tryParseCloseTag(fs, tagName, col, row)) {
            assertEx(!fs.is_end(), col, row);
            assertEx(childCount < maxChildren, col, row);
            children[childCount++] = parseNode(fs, col, row);
        }

        list<XMLNode> kids{childCount};
        for (size_type i = 0; i < childCount; i++) {
            kids[i] = static_cast<XMLNode&&>(children[i]);
        }
        list<XMLAttribute> atts{attCount};
        for (size_type i = 0; i < attCount; i++) {
            atts[i] = XMLAttribute(static_cast<string&&>(attNames[i]),
                                   static_cast<string&&>(attValues[i]));
        }
        return XMLNode(static_cast<string&&>(tagName),
                       static_cast<list<XMLAttribute>&&>(atts),
                       static_cast<list<XMLNode>&&>(kids));
    }

    XMLNode loadFromFile(file_stream& fs) throws
    {
        size_type col{}, row{};
        XMLNode out = parseNode(fs, col, row);
        skipWhitespace(fs, col, row);
        fs.read();
        assertEx(fs.is_end(), col, row);
        return out;
    }
}

static string* MultipleRootException_what{};
static string* BadSyntaxException_what{};

const string& MultipleRootException::what() const noexcept
{
    if (MultipleRootException_what == nullptr) {
        MultipleRootException_what = stdnew<string>("multiple root nodes");
    }
    return *MultipleRootException_what;
}

const string& BadSyntaxException::what() const noexcept
{
    if (BadSyntaxException_what == nullptr) {
        BadSyntaxException_what = stdnew<string>("bad syntax");
    }
    return *BadSyntaxException_what;
}
