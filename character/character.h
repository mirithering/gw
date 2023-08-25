#ifndef CHARACTER_H
#define CHARACTER_H

#include <bits/stdc++.h>

#include "action.h"
#include "armor/armor.h"
#include "base/attribute.h"
#include "base/clock.h"
#include "base/profession.h"
#include "skill.h"
#include "stance.h"
#include "weapon/weapon.h"

class Character : public TimedObject {
 public:
  Character(Profession first_profession);
  ~Character() = default;

  void Tick(int time_passed) override;

  bool ReceiveWeaponDamage(int damage, Weapon::Type type);
  void WeaponAttack(Character& target, int skill_damage = 0);

  void UseEnergy(int use_energy) {
    assert(energy_ >= use_energy);
    energy_ -= use_energy;
  }

  void RemoveOneAdrenalineStrike();

  void RechargeEnergy();

  void AddEnergy(int amount);

  Stance* SetStance(std::unique_ptr<Stance> stance) {
    stance_ = std::move(stance);
    return stance_.get();
  }

  void GiveWeapon(std::unique_ptr<Weapon> weapon) {
    weapon_ = std::move(weapon);
  }

  void SetAttribute(Attribute attribute, int num);
  void SetSkill(int pos, std::unique_ptr<Skill> skill);

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

  std::unique_ptr<Effect<Stance>> stance_;

  const int maxHealth_;
  int health_;
  int energy_;

  int attribute_points_;
};

std::ostream& operator<<(std::ostream& out, const Character& character);

#endif  // CHARACTER_H