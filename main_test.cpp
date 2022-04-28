#include "a.hpp"
#include "gtest/gtest.h"

TEST(FooTest, getByte)
{
	FooBar foo;
	char c[1] = {'a'};
	EXPECT_EQ('a', foo.getByte(c));
}

TEST(FooTest, readByte)
{
	FooBar foo;
	std::string str = "Hello, world!";
	foo.writeByte(str);
	EXPECT_EQ(str, foo.readByte());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
