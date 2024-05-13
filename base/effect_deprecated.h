#ifndef BASE_EFFECTDEPRECATED_H
#define BASE_EFFECTDEPRECATED_H

#include "clock.h"
#include "units.h"

// An EffectDeprecated contains an Object that will be removed once the duration
// is over.
// TODO putting a TimedObject into an effect has a bit of an off-by-one
// non-deterministic touch :/ You never know how much +/- 1 the Tick is called.
template <class T>
class EffectDeprecated : public TimedObject {
 public:
  // These identify the effect, use one type per kind of effect a character can
  // be affected with.
  enum class Type { EdgeOfExtinction };

  // TODO fix this, this is meant to be temporary while I get rid of this
  // overcomplication.
  virtual Type GetType() { return Type::EdgeOfExtinction; }

  EffectDeprecated() = default;
  EffectDeprecated(Time duration, std::unique_ptr<T> timed_object)
      : duration_(duration), timed_object_(std::move(timed_object)) {}

  EffectDeprecated(EffectDeprecated<T>&& other) {
    timed_object_ = std::move(other.timed_object_);
    duration_ = std::move(other.duration_);
  }
  EffectDeprecated<T>& operator=(EffectDeprecated<T>&& other) {
    timed_object_ = std::move(other.timed_object_);
    duration_ = std::move(other.duration_);
    return *this;
  }
  ~EffectDeprecated() override{};

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

  static EffectDeprecated<T> None() { return EffectDeprecated<T>(); }

 private:
  Time duration_;
  std::unique_ptr<T> timed_object_;
};

#endif  // BASE_EFFECT_H