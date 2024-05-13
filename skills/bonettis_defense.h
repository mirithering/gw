#ifndef SKILLS_BONETTIS_DEFENSE_H
#define SKILLS_BONETTIS_DEFENSE_H

#include <bits/stdc++.h>

#include "base/function_list.h"
#include "base/units.h"
#include "character/character.h"
#include "character/skill.h"
#include "character/stance.h"

class BonettisDefenseStance : public Stance {
 public:
  void AddModifiers(Character& character) override {
    modifier_ = {
        .character = &character,
        .block_modifier =
            character.callbacks_block_chance_.AddFunctionDeprecated(
                [](const Character& character, Weapon::Type type) {
                  if (IsMeele(type) || IsProjectile(type)) {
                    return Percent(75);
                  }
                  return Percent(0);
                }),
        .block_callback =
            character.callbacks_attack_blocked_.AddFunctionDeprecated(
                [](Character& character, Weapon::Type type) {
                  if (IsMeele(type)) {
                    character.AddEnergy(5);
                  }
                }),
    };
  }

  ~BonettisDefenseStance() override {
    if (modifier_.has_value()) {
      modifier_->character->callbacks_block_chance_.RemoveFunction(
          modifier_->block_modifier);
      modifier_->character->callbacks_attack_blocked_.RemoveFunction(
          modifier_->block_callback);
    }
  }

 private:
  struct Modifier {
    Character* character;
    FunctionList<Percent(const Character& character, Weapon::Type)>::ref
        block_modifier;
    FunctionList<void(Character& character, Weapon::Type)>::ref block_callback;
  };

  std::optional<Modifier> modifier_;
};

class BonettisDefense : public Skill {
 public:
  std::string Name() const override { return "Bonetti's Defense"; }

  Character* GetTarget(Character& character, World& world) const override {
    return &character;
  }
  Inches GetRange(const Character&) const override { return Inches(0); }

 protected:
  void ActivationEnd(Character& character, World& world) override;

  Adrenaline AdrenalineCost() const override { return 8 * Strike; }
  int EnergyCost() const override { return 0; }
  Time RechargeTime() const override { return Time(0); }
  Time ActivationTime(Character& character) const override { return Time(0); }
  // TODO create a stance skill class
  Skill::Type GetType() const override { return Skill::Type::Stance; }

 private:
  inline static const Attribute kAttribute = Attribute::Tactics;
  inline static const int kDuration[] = {5,  5,  6,  6,  7,  7,  7,  8,
                                         8,  9,  9,  9,  10, 10, 11, 11,
                                         11, 12, 12, 13, 13, 13};
};

#endif  // SKILLS_JAIZENJU_STRIKE_H