#include "pch.h"
#include "../Lab_2/Array.h"

TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 8);
}

TEST(ArrayTest, ConstructorWithCapacity) {
    const Array<int> arr(30);
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.capacity(), 30);
}

TEST(ArrayTest, Insert) {
    Array<int> arr;
    arr.insert(1);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 1);
}

TEST(ArrayTest, InsertAtIndex) {
    Array<int> arr;
    arr.insert(2);
    arr.insert(0, 1);
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(ArrayTest, Remove) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.remove(0);
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0], 2);
}

TEST(ArrayTest, Resize) {
    Array<int> arr(2);
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);
    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

TEST(ArrayTest, Index) {
    Array<int> arr;
    arr.insert(1);
    EXPECT_EQ(arr[0], 1);
    arr[0] = 2;
    EXPECT_EQ(arr[0], 2);
}

TEST(ArrayTest, CopyConstructor) {
    Array<int> arr;
    arr.insert(1);
    Array<int> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0], 1);
}

TEST(ArrayTest, MoveConstructor) {
    Array<int> arr;
    arr.insert(1);
    Array<int> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0], 1);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, AssignmentOperator) {
    Array<int> arr;
    arr.insert(1);
    Array<int> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0], 1);
}

TEST(ArrayTest, MoveAssignmentOperator) {
    Array<int> arr;
    arr.insert(1);
    Array<int> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0], 1);
    EXPECT_EQ(arr.size(), 0); 
}

TEST(ArrayTest, Iterator) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    auto it = arr.iterator();
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(it.hasNext());
        EXPECT_EQ(it.get(), i);
        it.next();
    }
    EXPECT_FALSE(it.hasNext());
}

TEST(ArrayTest, IteratorAfterModification) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    const auto it = arr.iterator();
    arr.insert(42);
    ASSERT_THROW(it.get(), std::runtime_error);
}

TEST(ArrayTest, ReverseIterator) {
    Array<int> arr;
    for (int i = 0; i < 5; ++i) {
        arr.insert(i);
    }
    auto it = arr.reverseIterator();
    for (int i = 4; i >= 0; --i) {
        ASSERT_TRUE(it.hasNext());
        EXPECT_EQ(it.get(), i);
        it.next();
    }
    EXPECT_FALSE(it.hasNext());
}

struct TestStruct {
    std::string s;
    explicit TestStruct(std::string  val) : s(std::move(val)) {}
    bool operator==(const TestStruct& other) const {
        return s == other.s;
    }
};

TEST(ArrayTest, InsertCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    EXPECT_EQ(arr.size(), 1);
    EXPECT_EQ(arr[0].s, "anel");
}

TEST(ArrayTest, RemoveCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    arr.remove(0);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayTest, IndexingCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    EXPECT_EQ(arr[0].s, "anel");
    arr[0].s = "biki";
    EXPECT_EQ(arr[0].s, "biki");
}

TEST(ArrayTest, CopyConstructorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    Array<TestStruct> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0].s, "anel");
}

TEST(ArrayTest, MoveConstructorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    Array<TestStruct> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0].s, "anel");
    EXPECT_EQ(arr.size(), 0); 
}

TEST(ArrayTest, AssignmentOperatorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    Array<TestStruct> arrCopy = arr;
    EXPECT_EQ(arrCopy.size(), 1);
    EXPECT_EQ(arrCopy[0].s, "anel");
}

TEST(ArrayTest, MoveAssignmentOperatorCustomType) {
    Array<TestStruct> arr;
    arr.insert(TestStruct("anel"));
    Array<TestStruct> arrMoved = std::move(arr);
    EXPECT_EQ(arrMoved.size(), 1);
    EXPECT_EQ(arrMoved[0].s, "anel");
    EXPECT_EQ(arr.size(), 0); 
}


