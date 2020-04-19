#include <stdexcept.hpp>
#include "xmlbench.hpp"
#include "std.hpp"
#include <stdio.h>
#include <string.h>

bool test(xmlb::XMLNode& doc) noexcept
{
    if (::strcmp(doc.tagName.c_str(), "root") != 0) return false;
    if (doc.children.size() != 0) return false;
    if (doc.attributes.size() != 1) return false;
    if (::strcmp(doc.attributes[0].name.c_str(), "att") != 0) return false;
    if (::strcmp(doc.attributes[0].value.c_str(), "1") != 0) return false;
    return true;
}

int main()
{
    try {
        file_stream fs{"test.xml"};
        xmlb::XMLNode doc = xmlb::loadFromFile(fs);
        if(test(doc)) {
            ::printf("Completed successfully.\n");
        }
        else {
            ::printf("Mismatching data.\n");
        }
    }
    catch (BadSyntaxException& e) {
        ::printf("Bad syntax at %lu:%lu\n", e.column, e.row);
    }
    catch (exception& e) {
        ::printf("Exception occurred: %s\n", e.what().c_str());
    }
}
