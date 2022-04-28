#include "a.hpp"

int main()
{
    FooBar foo;
    char c[1] = {'a'};
    std ::cout << foo.getByte(c) << std::endl;
    std::string str = "Hello, world!";
    foo.writeByte(str);
    std::cout << foo.readByte() << std::endl;
    return 0;
}