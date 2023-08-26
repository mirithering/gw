#ifndef CHARACTER_H
#define CHARACTER_H

#include <bits/stdc++.h>

#include "action.h"
#include "armor/armor.h"
#include "base/attribute.h"
#include "base/clock.h"
#include "base/effect.h"
#include "base/profession.h"
#include "condition.h"
#include "skill.h"
#include "stance.h"
#include "weapon/weapon.h"

class Character : public TimedObject {
 public:
  Character(Profession first_profession);
  ~Character() = default;

  void Tick(int time_passed) override;

  bool ReceiveWeaponDamage(int damage, Weapon::Type type,
                           bool blockable = true);
  bool WeaponAttack(Character& target, int skill_damage = 0,
                    bool blockable = true);

  void UseEnergy(int use_energy) {
    assert(energy_ >= use_energy);
    energy_ -= use_energy;
  }

  void AddHealth(int amount);
  void AddEnergy(int amount);

  void RemoveOneAdrenalineStrike();

  Stance* SetStance(Effect<Stance> stance) {
    stance_ = std::move(stance);
    return GetStance();
  }

  Effect<Condition>* AddCondition(Effect<Condition> condition) {
    conditions_.push_back(std::move(condition));
    return &conditions_.back();
  }

  const std::vector<Effect<Condition>>& GetConditions() const {
    return conditions_;
  }

  void GiveWeapon(std::unique_ptr<Weapon> weapon) {
    weapon_ = std::move(weapon);
  }

  void SetAttribute(Attribute attribute, int num);
  Skill* SetSkill(int pos, std::unique_ptr<Skill> skill);

  Action& GetAction() { return action_; }
  Skill* GetSkill(int pos) { return skills_[pos].get(); }

  Stance* GetStance() { return stance_.get(); }

  const Weapon& weapon() const { return *weapon_; }
  const Armor& armor() const { return *armor_; }
  int GetAttribute(Attribute attribute) const;
  Profession GetFirstProfession() const { return first_profession_; }
  int GetMaxHealth() const { return maxHealth_; }
  int health() const { return health_; }
  int energy() const { return energy_; }

  std::string name_ = "Lovely Princess";  // For debugging.

 private:
  void HealthGeneration();

  void EnergyGeneration();
  void AddAdrenaline(int charges);
  void Die();

  bool WillBlockAttack(Weapon::Type type);

  friend std::ostream& operator<<(std::ostream& out,
                                  const Character& character);

  Action action_ = kActionIdle;

  std::unique_ptr<Weapon> weapon_;
  std::unique_ptr<Armor> armor_;

  Profession first_profession_;

  std::map<Attribute, int> attributes_;

  std::unique_ptr<Skill> skills_[8];

  Effect<Stance> stance_ = Effect<Stance>::None();
  // TODO how to get rid of old conditions, and how to deal with duplicated
  // ones?
  std::vector<Effect<Condition>> conditions_;

  const int maxHealth_;
  int health_;
  int energy_;

  int attribute_points_;
};

std::ostream& operator<<(std::ostream& out, const Character& character);

#endif  // CHARACTER_H