#ifndef A_HPP
#define A_HPP
#include <iostream>
#include <fstream>

class FooBar
{
public:
	virtual char getByte(char *c);
	virtual void writeByte(const std::string &str);
	virtual std::string readByte();
};

class ReaderWriter
{
	FooBar *foo;

public:
	explicit ReaderWriter(FooBar *foo_) : foo(foo_) {}

	void operation()
	{
		std::string val = "Hello, world!";
		this->foo->getByte(const_cast<char *>(val.c_str()));
		this->foo->writeByte(val);
		this->foo->readByte();
	}
};

#endif