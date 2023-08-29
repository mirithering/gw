#ifndef BASE_EFFECT_H
#define BASE_EFFECT_H

#include "clock.h"
#include "units.h"

// An Effect contains an Object that will be removed once the duration is
// over.
// TODO putting a TimedObject into an effect has a bit of an off-by-one
// non-deterministic touch :/ You never know how much +/- 1 the Tick is called.
template <class T>
class Effect : public TimedObject {
 public:
  Effect() = default;
  Effect(Time duration, std::unique_ptr<T> timed_object)
      : duration_(duration), timed_object_(std::move(timed_object)) {}

  Effect(Effect<T>&& other) {
    timed_object_ = std::move(other.timed_object_);
    duration_ = std::move(other.duration_);
  }
  Effect<T>& operator=(Effect<T>&& other) {
    timed_object_ = std::move(other.timed_object_);
    duration_ = std::move(other.duration_);
    return *this;
  }
  ~Effect() override{};

  void Tick(Time) override {
    duration_--;
    if (duration_ <= Time(0)) {
      timed_object_.reset();
    }
  }

  bool Ended() const { return timed_object_ == nullptr; }
  Time RemainingDuration() const { return duration_; }

  T* get() { return timed_object_.get(); }
  const T* get() const { return timed_object_.get(); }

  static Effect<T> None() { return Effect<T>(); }

 private:
  Time duration_;
  std::unique_ptr<T> timed_object_;
};

#endif  // BASE_EFFECT_H