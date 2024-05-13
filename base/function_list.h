#ifndef BASE_FUNCTION_LIST_H
#define BASE_FUNCTION_LIST_H

#include <bits/stdc++.h>

template <class>
class FunctionList;

// TODO I think this is just a fancy re-implementation of what weak_ptr and
// shared_ptr together can achieve.
template <class R, class... Args>
class FunctionList<R(Args...)> {
 public:
  typedef std::function<R(Args...)> func;
  typedef std::_List_iterator<func> ref;

  // A reference to an added function. The function is removed from the list
  // once the reference is deleted.
  class UniqueReference {
   public:
    UniqueReference() = default;
    UniqueReference(FunctionList<R(Args...)>* list, ref modifier)
        : list_(list), modifier_(modifier) {}
    UniqueReference(const UniqueReference&) = delete;
    UniqueReference& operator=(const UniqueReference&) = delete;
    UniqueReference(UniqueReference&& other) { *this = other; }
    UniqueReference& operator=(UniqueReference&& other) {
      list_ = other.list_;
      modifier_ = other.modifier_;
      other.list_ = nullptr;
      return *this;
    }

    ~UniqueReference() {
      if (list_) {
        list_->RemoveFunction(modifier_);
      };
    }

   private:
    FunctionList<R(Args...)>* list_ = nullptr;
    FunctionList<R(Args...)>::ref modifier_;
  };

  FunctionList() = default;
  FunctionList(const FunctionList& other) = delete;
  FunctionList& operator=(const FunctionList& other) = delete;
  FunctionList(FunctionList&& other) = default;
  FunctionList& operator=(FunctionList&& other) = default;

  ~FunctionList() { assert(list_.empty()); }

  ref AddFunctionDeprecated(func f) {
    list_.push_back(f);
    return --list_.end();
  }

  UniqueReference AddFunction(func f) {
    list_.push_back(f);
    return UniqueReference(this, --list_.end());
  }

  void RemoveFunction(std::_List_iterator<func> reference) {
    list_.erase(reference);
  }

  const std::list<func>& GetList() const { return list_; }

 private:
  std::list<func> list_;
};

#endif  // BASE_FUNCTION_LIST_H