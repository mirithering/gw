#ifndef SKILLS_BONETTIS_DEFENSE_H
#define SKILLS_BONETTIS_DEFENSE_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "entities/creature.h"
#include "entities/skill.h"
#include "entities/stance.h"

class BonettisDefenseStance : public Stance {
 public:
  void AddModifiers(Creature& creature) override {
    modifier_ = {
        .creature = &creature,
        .block_modifier =
            creature.callbacks_block_chance_.AddFunctionDeprecated(
                [](const Creature& creature, Weapon::Type type) {
                  if (IsMeele(type) || IsProjectile(type)) {
                    return Percent(75);
                  }
                  return Percent(0);
                }),
        .block_callback =
            creature.callbacks_attack_blocked_.AddFunctionDeprecated(
                [](Creature& creature, Weapon::Type type) {
                  if (IsMeele(type)) {
                    creature.AddEnergy(5);
                  }
                }),
    };
  }

  ~BonettisDefenseStance() override {
    if (modifier_.has_value()) {
      modifier_->creature->callbacks_block_chance_.RemoveFunction(
          modifier_->block_modifier);
      modifier_->creature->callbacks_attack_blocked_.RemoveFunction(
          modifier_->block_callback);
    }
  }

 private:
  struct Modifier {
    Creature* creature;
    FunctionList<Percent(const Creature& creature, Weapon::Type)>::ref
        block_modifier;
    FunctionList<void(Creature& creature, Weapon::Type)>::ref block_callback;
  };

  std::optional<Modifier> modifier_;
};

class BonettisDefense : public Skill {
 public:
  std::string Name() const override { return "Bonetti's Defense"; }

  Creature* GetTarget(Creature& creature, World& world) const override {
    return &creature;
  }
  Inches GetRange(const Creature&) const override { return Inches(0); }

 protected:
  void ActivationEnd(Creature& creature, World& world) override;

  Adrenaline AdrenalineCost() const override { return 8 * Strike; }
  int EnergyCost() const override { return 0; }
  Time RechargeTime() const override { return Time(0); }
  Time ActivationTime(Creature& creature) const override { return Time(0); }
  // TODO create a stance skill class
  Skill::Type GetType() const override { return Skill::Type::Stance; }

 private:
  inline static const Attribute kAttribute = Attribute::Tactics;
  inline static const int kDuration[] = {5,  5,  6,  6,  7,  7,  7,  8,
                                         8,  9,  9,  9,  10, 10, 11, 11,
                                         11, 12, 12, 13, 13, 13};
};

#endif  // SKILLS_JAIZENJU_STRIKE_H