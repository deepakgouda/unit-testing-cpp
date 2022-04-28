#include "a.hpp"
#include "gtest/gtest.h"

class FooBarTest:public ::testing::Test
{
	protected:
	FooBarTest(){}
	virtual ~FooBarTest(){}
	void SetUp(){}
	void TearDown(){}
	FooBar foo;
};

TEST_F(FooBarTest, getByte)
{
	char c[1] = {'a'};
	EXPECT_EQ('a', foo.getByte(c));
}