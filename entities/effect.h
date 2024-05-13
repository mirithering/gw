#ifndef ENTITIES_EFFECT_H
#define ENTITIES_EFFECT_H

#include <bits/stdc++.h>

#include "base/clock.h"

/**
 * An effect is something applies to a creature.
 * A creature can always have at most one instance of a certain effect type.
 * E.g. "burning", or "imagined burden".
 * Effect has a beginning and an end.
 * Override functions are called at the beginning, during, and at the end of an
 * effect. After an effect ends, it should be ready to be deallocated.
 */
class Effect : public TimedObject {
 public:
  enum class Type {
    Stance,
    // Conditions
    Bleeding,
    Crippled,
    Dazed,
    DeepWound,
    // Hexes
    FeveredDreams,
    ImaginedBurden,
    SoothingImages,
    // Spirit areal effects
    EdgeOfExtinction,
  };

  explicit Effect(Time duration) : duration_(duration) {
    // An effect with no duration makes no sense.
    assert(duration > Time(0));
  };

  void Tick(Time time_passed) override final {
    if (ended_) {
      return;
    }
    if (time_passed == Time(1)) {
      Start();
    }
    Tick();
    if (time_passed >= duration_) {
      ended_ = true;
      End();
    }
  }

  bool Ended() const { return ended_; }
  // TODO I wonder if I need to allow for "ending" an effect before duration is
  // over.

 private:
  // Override these for a concrete effect

  // Called with the first tick of the effect.
  virtual void Start() = 0;
  // Called on each tick during lifetime.
  virtual void Tick() = 0;
  // Called with the last tick of the effect.
  virtual void End() = 0;

  bool ended_ = false;
  Time duration_;
};

#endif