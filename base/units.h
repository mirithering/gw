#ifndef BASE_UNITS_H
#define BASE_UNITS_H

#include <bits/stdc++.h>

enum class UnitType { Time, Adrenaline, Health, Energy };

template <UnitType type>
class Unit {
 public:
  // TODO Ideally I would hide this constructor as private.
  explicit Unit(int value) : value_(value){};
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

  friend Unit operator*(Unit lhs, int rhs) { return Unit(lhs.value_ * rhs); }
  friend Unit operator*(int lhs, Unit rhs) { return Unit(lhs * rhs.value_); };
  friend Unit operator/(Unit lhs, int rhs) { return Unit(lhs.value_ / rhs); }
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

  int value() const { return value_; }

 private:
  int value_;
};

using Time = Unit<UnitType::Time>;

const Time Second = Time(1000);
const Time Millisecond = Time(1);
const Time Eternity = Time(INT_MAX);

using Adrenaline = Unit<UnitType::Adrenaline>;

const Adrenaline Strike = Adrenaline(25);

using Health = Unit<UnitType::Health>;

using Energy = Unit<UnitType::Energy>;

#endif  // BASE_UNITS_H