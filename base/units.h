#ifndef BASE_UNITS_H
#define BASE_UNITS_H

#include <bits/stdc++.h>

class Time {
 public:
  // TODO Ideally I would hide this constructor as private.
  explicit Time(int value) : value_(value){};
  Time() : value_(0){};
  int milliseconds() const { return value_; }

  // prefix increment
  Time& operator++() {
    ++value_;
    return *this;
  }
  // postfix increment
  Time operator++(int) {
    Time old = *this;  // copy old value
    operator++();      // prefix increment
    return old;        // return old value
  }

  friend Time operator*(Time lhs, unsigned int rhs) {
    return Time(lhs.value_ * rhs);
  }
  friend Time operator*(unsigned int lhs, Time rhs) {
    return Time(lhs * rhs.value_);
  };
  friend Time operator+(Time lhs, Time rhs) {
    return Time(lhs.value_ + rhs.value_);
  };
  friend Time operator-(Time lhs, Time rhs) {
    return Time(lhs.value_ - rhs.value_);
  };

  // Since C++20. Results in all comparison operators to be defined. :)
  auto operator<=>(const Time&) const = default;

 private:
  friend Time operator*(Time lhs, unsigned int rhs);
  friend Time operator*(unsigned int lhs, Time rhs);
  friend Time operator+(Time lhs, Time rhs);

  int value_;
};

const Time Second = Time(1000);
const Time Millisecond = Time(1);

#endif  // BASE_UNITS_H