#ifndef BASE_EVENT_H
#define BASE_EVENT_H

#include <bits/stdc++.h>

#include "clock.h"
#include "effect_deprecated.h"

template <class... Args>
class FunctionResolver {
 public:
  FunctionResolver(std::function<void(Args...)> func) : func_(func) {}
  virtual ~FunctionResolver() { func_(); }

  void Disarm() {
    func_ = [](Args...) {};
  }

 private:
  std::function<void(Args...)> func_;
};

// An event contains a callback that will be run once the time is over.
// If the event is destroyed before time is over, nothing happens.
template <class... Args>
class Event {
 public:
  Event(Time until_event, std::function<void(Args...)> effect)
      : effect_(EffectDeprecated<FunctionResolver<Args...>>(
            until_event, std::make_unique<FunctionResolver<Args...>>(effect))) {
  }
  Event(Event&&) = default;
  Event& operator=(Event&&) = default;
  virtual ~Event() {
    if (effect_.get()) {
      effect_.get()->Disarm();
    }
  }

 private:
  EffectDeprecated<FunctionResolver<Args...>> effect_;
};

#endif  // BASE_EVENT_H