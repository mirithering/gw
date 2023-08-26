#ifndef SKILLS_JAIZENJU_STRIKE_H
#define SKILLS_JAIZENJU_STRIKE_H

#include "character/skill.h"
#include "pure_strike.h"

// Duplicate skill.
class JaizenjuStrike : public PureStrike {
 public:
  std::string Name() const override { return "Jaizenju Strike"; }
};

#endif  // SKILLS_JAIZENJU_STRIKE_H