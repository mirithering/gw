#include "function_list.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

TEST(FunctionListTest, CanNotDeleteEmptyList) {
  auto* list = new FunctionList<void(void)>();
  list->AddFunction([]() {});
  ASSERT_DEATH(delete list, "empty");
}

TEST(FunctionListTest, AddAndRemoveElement) {
  FunctionList<void(void)> list;
  int called = 0;
  auto ref = list.AddFunction([&]() { called++; });
  ASSERT_EQ(list.GetList().size(), 1);
  list.GetList().front()();
  ASSERT_EQ(called, 1);
  list.RemoveFunction(ref);
  ASSERT_EQ(list.GetList().size(), 0);
}

TEST(FunctionListTest, ListCanBeMoved) {
  FunctionList<void(void)> list;
  int called = 0;
  auto ref = list.AddFunction([&]() { called++; });
  ASSERT_EQ(list.GetList().size(), 1);
  list.GetList().front()();
  ASSERT_EQ(called, 1);

  FunctionList<void(void)> moved_list = std::move(list);

  moved_list.GetList().front()();
  ASSERT_EQ(called, 2);

  moved_list.RemoveFunction(ref);
  ASSERT_EQ(moved_list.GetList().size(), 0);
}

TEST(FunctionListTest, MultipleFunctionsAreCalledInAddOrder) {
  FunctionList<int(int)> list;

  auto ref1 = list.AddFunction([](int a) { return a + 1; });
  auto ref2 = list.AddFunction([](int b) { return b + 3; });

  int x = 0;
  auto it = list.GetList().begin();
  x = (*it)(x);
  ASSERT_EQ(x, 1);
  it++;
  x = (*it)(x);
  ASSERT_EQ(x, 4);

  list.RemoveFunction(ref1);
  list.RemoveFunction(ref2);
}