#include "a.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class FooBarMock : public FooBar
{
public:
	MOCK_METHOD(char, getByte, (char *), (override));
	MOCK_METHOD(void, writeByte, (const std::string &), (override));
	MOCK_METHOD(std::string, readByte, (), (override));
};

using ::testing::AtLeast;
using ::testing::Return;

TEST(FooBarMock, CanReadFile)
{
	std::shared_ptr<FooBar>foo(new FooBarMock);
	EXPECT_CALL(*std::static_pointer_cast<FooBarMock>(foo), readByte()).Times(AtLeast(1));
	ReaderWriter rw(foo.get());
	rw.operation();
}
