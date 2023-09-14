#ifndef CHARACTER_HEX_H
#define CHARACTER_HEX_H

#include <bits/stdc++.h>

class Creature;

// TODO move all of this into a Modifier class and have only Effect::Type for
// stances, conitions, hexes, boons etc.
class Hex {
 public:
  Hex() = default;
  Hex(const Hex&) = delete;
  Hex& operator=(const Hex&) = delete;
  virtual ~Hex() = default;

  virtual void AddModifiers(Creature& creature) = 0;
  enum class Type { ImaginedBurden };
  virtual Type GetType() const = 0;
};

#endif  // CHARACTER_HEX_H