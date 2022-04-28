#include "a.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class FakeFooBar : public FooBar
{
public:
	std::string readByte() override
	{
		return "Child hello!";
	}
};
class MockFooBar : public FooBar
{
	FakeFooBar fake_;

public:
	MOCK_METHOD(char, getByte, (char *), (override));
	MOCK_METHOD(void, writeByte, (const std::string &), (override));
	MOCK_METHOD(std::string, readByte, (), (override));

	void DelegateToFake()
	{
		ON_CALL(*this, readByte).WillByDefault([this]()
											   { return fake_.readByte(); });
	}
};

using ::testing::AtLeast;
using ::testing::Return;

TEST(MockFooBar, CanReadFile)
{
	std::shared_ptr<FooBar>foo(new MockFooBar);
	EXPECT_CALL(*std::static_pointer_cast<MockFooBar>(foo), readByte()).Times(AtLeast(1));
	EXPECT_CALL(*std::static_pointer_cast<MockFooBar>(foo), writeByte).Times(AtLeast(1));
	EXPECT_CALL(*std::static_pointer_cast<MockFooBar>(foo), getByte).Times(AtLeast(1));
	ReaderWriter rw(foo.get());
	rw.operation();
	foo->FooBar::writeByte("Hello, world!");
	EXPECT_EQ("Hello, world!", foo->FooBar::readByte());
}

TEST(FakeFooBar, CanFake)
{
	std::shared_ptr<MockFooBar> foo(new MockFooBar);
	foo->DelegateToFake();
	EXPECT_CALL(*foo, readByte()).Times(AtLeast(1));
	EXPECT_CALL(*foo, writeByte).Times(AtLeast(1));
	EXPECT_CALL(*foo, getByte).Times(AtLeast(1));
	ReaderWriter rw(foo.get());
	rw.operation();
	foo->writeByte("Hello, world!");
	EXPECT_EQ("Child hello!", foo->readByte());
}