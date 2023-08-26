#ifndef BASE_EFFECT_H
#define BASE_EFFECT_H

#include "clock.h"

// An Effect contains an Object that will be removed once the duration is
// over.
// TODO putting a TimedObject into an effect has a bit of an off-by-one
// non-deterministic touch :/ You never know how much +/- 1 the Tick is called.
template <class T>
class Effect : public TimedObject {
 public:
  Effect(int duration, std::unique_ptr<T> timed_object)
      : duration_(duration), timed_object_(std::move(timed_object)) {}

  Effect(Effect<T>&&) = default;
  Effect<T>& operator=(Effect<T>&&) = default;
  ~Effect() override{};

  void Tick(int time) override {
    if (duration_ <= time) {
      timed_object_.reset();
    }
  }

  bool Ended() { return timed_object_ == nullptr; }

  T* get() { return timed_object_.get(); }
  const T* get() const { return timed_object_.get(); }

  static Effect<T> None() { return Effect<T>(); }

 private:
  Effect() = default;
  int duration_ = 0;
  std::unique_ptr<T> timed_object_;
};

#endif  // BASE_EFFECT_H