#ifndef BASE_UNITS_H
#define BASE_UNITS_H

#include <bits/stdc++.h>

enum class UnitType { Time, Adrenaline, Health, Energy, Percent };

template <UnitType type, class ValueType>
class Unit {
 public:
  explicit Unit(ValueType value) : value_(value){};
  Unit() : value_(0){};

  // prefix increment
  Unit& operator++() {
    ++value_;
    return *this;
  }
  // postfix increment
  Unit operator++(int) {
    Unit old = *this;  // copy old value
    operator++();      // prefix increment
    return old;        // return old value
  }
  // prefix decrement
  Unit& operator--() {
    --value_;
    return *this;
  }
  // postfix decrement
  Unit operator--(int) {
    Unit old = *this;  // copy old value
    operator--();      // prefix decrement
    return old;        // return old value
  }

  Unit& operator+=(const Unit& rhs) {
    value_ += rhs.value();
    return *this;
  }

  Unit& operator-=(const Unit& rhs) {
    value_ -= rhs.value();
    return *this;
  }

  friend Unit operator*(Unit lhs, ValueType rhs) {
    return Unit(lhs.value_ * rhs);
  }
  friend Unit operator*(ValueType lhs, Unit rhs) {
    return Unit(lhs * rhs.value_);
  };
  friend Unit operator/(Unit lhs, ValueType rhs) {
    return Unit(lhs.value_ / rhs);
  }
  friend ValueType operator/(Unit lhs, Unit rhs) {
    return lhs.value_ / rhs.value_;
  }
  friend Unit operator+(Unit lhs, Unit rhs) {
    return Unit(lhs.value_ + rhs.value_);
  };
  friend Unit operator-(Unit lhs, Unit rhs) {
    return Unit(lhs.value_ - rhs.value_);
  };
  friend Unit operator%(Unit lhs, Unit rhs) {
    return Unit(lhs.value_ % rhs.value_);
  };
  friend std::ostream& operator<<(std::ostream& out, const Unit& unit) {
    out << unit.value();
    return out;
  }

  // Since C++20. Results in all comparison operators to be defined. :)
  auto operator<=>(const Unit&) const = default;

  ValueType value() const { return value_; }

 private:
  ValueType value_;
};

using Time = Unit<UnitType::Time, int>;

const Time Second = Time(1000);
const Time Millisecond = Time(1);
const Time Eternity = Time(INT_MAX);

using Adrenaline = Unit<UnitType::Adrenaline, int>;

const Adrenaline Strike = Adrenaline(25);

using Health = Unit<UnitType::Health, double>;
using Energy = Unit<UnitType::Energy, double>;

using Percent = Unit<UnitType::Percent, double>;

template <class T>
T of(T value, Percent percent) {
  assert(percent.value() >= 0 && percent.value() <= 100);
  return value * percent.value() / 100.0;
}

#endif  // BASE_UNITS_H