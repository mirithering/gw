#ifndef BASE_FUNCTION_LIST_H
#define BASE_FUNCTION_LIST_H

#include <bits/stdc++.h>

template <class>
class FunctionList;

template <class R, class... Args>
class FunctionList<R(Args...)> {
 public:
  typedef std::function<R(Args...)> func;
  typedef std::_List_iterator<func> ref;

  FunctionList() = default;
  FunctionList(const FunctionList& other) = delete;
  FunctionList& operator=(const FunctionList& other) = delete;
  FunctionList(FunctionList&& other) = default;
  FunctionList& operator=(FunctionList&& other) = default;

  ~FunctionList() { assert(list_.empty()); }

  ref AddFunction(func f) {
    list_.push_back(f);
    return --list_.end();
  }

  void RemoveFunction(ref reference) { list_.erase(reference); }

  const std::list<func>& GetList() const { return list_; }

 private:
  std::list<func> list_;
};

#endif  // BASE_FUNCTION_LIST_H