#ifndef BASE_CLOCK_H
#define BASE_CLOCK_H

#include <bits/stdc++.h>

// There is no guarantee time will be 0 at any point, the only guarantee is that
// it is increased by one on every tick. Only use relatively.
class TimedObject {
 public:
  TimedObject();
  virtual ~TimedObject();

  virtual void Tick(int time_passed) = 0;

 private:
  std::function<void(int time)> tick_;

  int creation_time_ = 0;
};

// An Effect contains a TimedObject that will be removed once the duration is
// over.
template <class T>
class Effect : public TimedObject {
 public:
  Effect(int duration, std::unqiue_ptr<T> timed_object)
      : duration_(duration), timed_object_(timed_object) {}

  virtual ~Effect() = default;

  void Tick(int time) override {
    if (duration_ < time) {
      timed_object_.reset();
      return;
    }
    timed_object_->Tick();
  }

  bool Ended() { return timed_object_ == nullptr; }

  T* get() { return timed_object_get(); }

 private:
  int duration_ = 0;
  std::unique_ptr<T> timed_object_;
};

void Tick();

#endif  // BASE_CLOCK_H