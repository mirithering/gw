#ifndef SKILL_H
#define SKILL_H

#include <bits/stdc++.h>

#include "base/clock.h"
#include "character/world.h"

class Action;
class Character;

// TODO implement aftercast delay
class Skill : public TimedObject {
 public:
  enum class Type {
    Spell,
    HexSpell,
    ItemSpell,
    WardSpell,
    WeaponSpell,
    WellSpell,
    Attack,
    Stance,
  };

  Skill() = default;

  virtual bool CanActivate(Character& character, World& world) const;
  virtual ~Skill() override = default;

  Action Activate(Character& character, World& world);

  void Tick(Time) override final {
    recharge_ = std::max(Time(0), recharge_ - Millisecond);
  }

  void AddAdrenaline(Adrenaline adrenaline);
  void LoseAdrenaline(Adrenaline adrenaline);
  void LoseAllAdrenaline();
  Adrenaline GetAdrenaline() const;

  virtual std::string Name() const = 0;

  virtual Character* GetTarget(Character& character, World& world) const = 0;

  virtual Inches GetRange(const Character& character) const = 0;

 protected:
  Character* target_ = nullptr;
  virtual Adrenaline AdrenalineCost() const = 0;
  virtual int EnergyCost() const = 0;
  virtual Time RechargeTime() const = 0;
  virtual Time ActivationTime(Character& character) const = 0;
  virtual Type GetType() const = 0;

  virtual void ActivationStart(Character& character, World& world);
  virtual void ActivationMiddle(Character& character, World& world){};
  virtual void ActivationEnd(Character& character, World& world);

 private:
  Adrenaline adrenaline_ =
      Adrenaline(0);  // In theory, adrenaline is also lost when being idle.
                      // But I don't think I care.
  Time recharge_ = Time(0);
};

bool IsSpell(Skill::Type type);

#endif