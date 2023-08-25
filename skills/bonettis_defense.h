#ifndef SKILLS_BONETTIS_DEFENSE_H
#define SKILLS_BONETTIS_DEFENSE_H

#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"

class BonettisDefenseStance : public Stance {
 public:
  BonettisDefenseStance(int duration, Character& character)
      : Stance(duration), character_(character) {}

  virtual int BlockChance(Weapon::Type type) override;
  virtual void AttackBlocked(Weapon::Type type) override;

 private:
  Character& character_;
};

class BonettisDefense : public Skill {
 public:
  bool CanUse(const Character& character) const override {
    return Skill::CanUse(character);
  };

  Action Use(Character& source, Character& target) override;

  std::string Name() override { return "Bonetti's Defense"; }

  int AdrenalineCost() const override { return 8 * 25; }

 private:
  inline static const Attribute attribute = Attribute::Tactics;
  inline static const int duration[] = {5,  5,  6,  6,  7,  7,  7,  8,
                                        8,  9,  9,  9,  10, 10, 11, 11,
                                        11, 12, 12, 13, 13, 13};
};

#endif  // SKILLS_JAIZENJU_STRIKE_H