# Unit testing in C++ using GoogleTest and gMock
## Installation
```bash
git clone https://github.com/google/googletest.git -b release-1.11.0
cd googletest
mkdir build
cd build
cmake ..
make
make install
```

## Sample binary
```cpp
// a.hpp
#ifndef A_HPP
#define A_HPP

#include <iostream>
#include <fstream>

char getByte(char *c);
void writeByte(const std::string &str);
std::string readByte();
#endif
```

```cpp
// a.cpp
#include "a.hpp"

char getByte(char *c)
{
	std::cout << c[0] << std::endl;
	return c[0];
}

void writeByte(const std::string &str)
{
	std::ofstream file_wrtr;
	file_wrtr.open("dump.log");
	file_wrtr << str;
	file_wrtr.close();
}

std::string readByte()
{
	std::string line;
	std::ifstream file_rdr("dump.log");
	if (file_rdr.is_open())
	{
		while (std::getline(file_rdr, line))
		{
			std::cout << line << '\n';
		}
		file_rdr.close();
	}
	return line;
}
```

```cpp
// main_test.cpp
#include "a.hpp"
#include "gtest/gtest.h"

TEST(DummyTest, getByte)
{
	char c[1] = {'a'};
	EXPECT_EQ('a', getByte(c));
}

TEST(DummyTest, readByte)
{
	std::string str = "Hello, world!";
	writeByte(str);
	EXPECT_EQ(str, readByte());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
```

```cpp
// main.cpp
#include "a.hpp"

int main()
{
	char c[1] = {'a'};
	std ::cout << getByte(c) << std::endl;
	std::string str = "Hello, world!";
	writeByte(str);
	std::cout << readByte() << std::endl;
	return 0;
}
```

## CMake configuration
```cmake
cmake_minimum_required(VERSION 3.15.0)
project(Test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FILES_TO_TEST a.cpp)
set(UNIT_TESTS main_test.cpp)
set(MAIN_FILE main.cpp)

add_subdirectory(googletest/)
include_directories(googletest/include)
add_library(files_to_test ${FILES_TO_TEST})

enable_testing()
add_executable(main ${MAIN_FILE})
target_link_libraries(main files_to_test)

add_executable(unit_test ${UNIT_TESTS})
target_link_libraries(unit_test gtest gtest_main rt pthread files_to_test)

include(GoogleTest)
gtest_discover_tests(unit_test)
```

Run using  `ctest` or `./unit_test`

## Basics
* _Assertions_ - statements that check whether a condition is true. Assertion’s result can be _success_, _nonfatal failure_, or _fatal failure_. If a fatal failure occurs, it aborts the current function; otherwise the program continues normally.
* A _test suite_ contains one or many tests. You should group your tests into test suites that reflect the structure of the tested code. When multiple tests in a test suite need to share common objects and subroutines, you can put them into a _test fixture_ class.
* A _test program_ can contain multiple test suites.
* `EXPECT_*` versions generate nonfatal failures, which don’t abort the current function.
* `ASSERT_*` versions generate fatal failures when they fail, and **abort the current function**.
* `ASSERT_*` may cause a space leak as it returns from the current function immediately, possibly skipping clean-up code. Hence, you may get a heap checker error in addition to assertion errors.
* Providing custom failure message :
	```cpp
	ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";
	```

## Creating a Test
`TEST()` - These are ordinary C++ functions that don’t return a value.

```cpp
TEST(TestSuiteName, TestName)
{
  ... test body ...
}
```

**Note** : `TestSuiteName` and `TestName` names must be valid C++ identifiers, and they should not contain any underscores (`_`)

## Creating a Test Fixture
If you find yourself writing two or more tests that operate on similar data, you can use a _test fixture_. This allows you to reuse the same configuration of objects for several different tests.

To create a fixture:
1.  Derive a class from `::testing::Test` . Start its body with `protected:`, as we’ll want to access fixture members from sub-classes.
2.  Inside the class, declare any objects you plan to use.
3.  If necessary, write a default constructor or `SetUp()` function to prepare the objects for each test. Use `override` in C++11 to make sure you spelled it correctly.
4.  If necessary, write a destructor or `TearDown()` function to release any resources you allocated in `SetUp()` . To learn when you should use the constructor/destructor and when you should use `SetUp()/TearDown()`, read the [FAQ](https://google.github.io/googletest/faq.html#CtorVsSetUp).
5.  If needed, define subroutines for your tests to share.

```cpp
TEST_F(TestFixtureName, TestName)
{
  ... test body ...
}
```

**Note** :  First define a test fixture class before using it in a `TEST_F()`, or you’ll get the compiler error “`virtual outside class declaration`”.

For each test defined with `TEST_F()`, googletest will create a _fresh_ test fixture at runtime, immediately initialize it via `SetUp()`, run the test, clean up by calling `TearDown()`, and then delete the test fixture.

*googletest* does **not** reuse the same test fixture for multiple tests.

### Example 
```cpp
template <typename E>  // E is the element type.
class Queue {
	public:
	Queue();
	void Enqueue(const E& element);
	E* Dequeue();  // Returns NULL if the queue is empty.
	size_t size() const;
	...
};
```
```cpp
class QueueTest : public ::testing::Test {
	protected:
	void SetUp() override
	{
		q1_.Enqueue(1);
		q2_.Enqueue(2);
		q2_.Enqueue(3);
	}

	// void TearDown() override {}
	Queue<int> q0_;
	Queue<int> q1_;
	Queue<int> q2_;
};

TEST_F(QueueTest, IsEmptyInitially)
{
	EXPECT_EQ(q0_.size(), 0);
}

TEST_F(QueueTest, DequeueWorks)
{
	int* n = q0_.Dequeue();
	EXPECT_EQ(n, nullptr);
	
	n = q1_.Dequeue();
	ASSERT_NE(n, nullptr);
	EXPECT_EQ(*n, 1);
	EXPECT_EQ(q1_.size(), 0);
	delete n;
	
	n = q2_.Dequeue();
	ASSERT_NE(n, nullptr);
	EXPECT_EQ(*n, 2);
	EXPECT_EQ(q2_.size(), 1);
	delete n;
}
```

**Note** : 
* You must **not** ignore the return value of `RUN_ALL_TESTS()`, or you will get a compiler error.
* You should call `RUN_ALL_TESTS()` only **once**. Calling it more than once conflicts with some advanced googletest features

## gMock
When you write a prototype or test, often it’s not feasible or wise to rely on real objects entirely. A **mock object** implements the same interface as a real object (so it can be used as one), but lets you specify at run time how it will be used and what it should do (which methods will be called? in which order? how many times? with what arguments? what will they return? etc).

### Sample
```cpp
// gmock_test.cpp
#include "a.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class FooBarMock : public FooBar
{
public:
	MOCK_METHOD(char, getByte, (char *));
	MOCK_METHOD(void, writeByte, (const std::string &));
	MOCK_METHOD(std::string, readByte, ());
};

using ::testing::AtLeast;

TEST(FooBarMock, CanReadFile)
{
	FooBarMock foo;
	EXPECT_CALL(foo, readByte()).Times(AtLeast(1));
	foo.writeByte("Hello, world!");
	foo.readByte();
}
```

**Note** : gMock requires expectations to be set **before** the mock functions are called, otherwise the behavior is **undefined**.

## Delegating calls to Fake class
```cpp
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
private:
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
```
**Note** : A call to `DelegateToFake()` is necessary to enable the delegation to fake functions

## Source
1. [googletest primer](https://google.github.io/googletest/primer.html)
2. [gMock](https://google.github.io/googletest/gmock_for_dummies.html)
3. [Delegation to fake](https://google.github.io/googletest/gmock_cook_book.html#DelegatingToFake)
4. 