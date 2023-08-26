#ifndef SKILLS_BONETTIS_DEFENSE_H
#define SKILLS_BONETTIS_DEFENSE_H

#include <bits/stdc++.h>

#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"

class BonettisDefenseStance : public Stance {
 public:
  BonettisDefenseStance(Character& character) : character_(character) {}

  virtual int BlockChance(Weapon::Type type) override;
  virtual void AttackBlocked(Weapon::Type type) override;

 private:
  Character& character_;
};

class BonettisDefense : public Skill {
 public:
  std::string Name() const override { return "Bonetti's Defense"; }

 protected:
  virtual void ActivationEnd(Character& character) override;

  int AdrenalineCost() const override { return 8 * 25; }
  int EnergyCost() const override { return 0; }
  int RechargeTime() const override { return 0; }
  int ActivationTime(Character& character) const override { return 0; }

 private:
  inline static const Attribute kAttribute = Attribute::Tactics;
  inline static const int kDuration[] = {5,  5,  6,  6,  7,  7,  7,  8,
                                         8,  9,  9,  9,  10, 10, 11, 11,
                                         11, 12, 12, 13, 13, 13};
};

#endif  // SKILLS_JAIZENJU_STRIKE_H