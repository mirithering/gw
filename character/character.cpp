#include "character.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/profession.h"
#include "base/random.h"
#include "damage.h"
#include "skill.h"

namespace {

std::map<Profession, int> kMaxEnergy = {
    {Profession::Warrior, 20},     {Profession::Ranger, 25},
    {Profession::Dervish, 25},     {Profession::Assassin, 25},
    {Profession::Paragon, 30},     {Profession::Elementalist, 30},
    {Profession::Monk, 30},        {Profession::Mesmer, 30},
    {Profession::Necromancer, 30}, {Profession::Ritualist, 30}};

// 1 means 1 energy every 3 seconds.
std::map<Profession, int> kEnergyRegeneration = {
    {Profession::Warrior, 2},     {Profession::Ranger, 3},
    {Profession::Dervish, 4},     {Profession::Assassin, 4},
    {Profession::Paragon, 2},     {Profession::Elementalist, 4},
    {Profession::Monk, 4},        {Profession::Mesmer, 4},
    {Profession::Necromancer, 4}, {Profession::Ritualist, 4}};

constexpr int kMaxHealth = 480;  // At level 20
}  // namespace

Character::Character(Profession first_profession) {
  first_profession_ = first_profession;
  armor_ = std::make_unique<Armor>(first_profession);
  health_lost_ = 0;
  energy_ = kMaxEnergy[first_profession];

  attribute_points_ = 200;
}

void Character::Tick(int time_passed) {
  if (time_passed % 3000 == 0) {
    EnergyGeneration();
  }
  if (time_passed % 1000 == 0) {
    HealthGeneration();
  }

  if (GetAction().Tick() == Action::Result::End) {
    GetAction() = kActionIdle;
  }

  // TODO Remove ended conditions.
}

void Character::HealthGeneration() {
  int health_generation = 0;  // Base is 0.

  for (auto& condition : conditions_) {
    if (!condition.Ended()) {
      health_generation += condition.get()->HealthGeneration();
    }
  }

  health_generation =
      std::max(-10, health_generation);  // Negative generation is capped at 10.
  LoseHealth(-(2 * health_generation));
}

void Character::EnergyGeneration() {
  int energy_generation = kEnergyRegeneration.at(first_profession_);
  AddEnergy(energy_generation);
}

void Character::AddEnergy(int amount) {
  energy_ = std::min(kMaxEnergy.at(first_profession_), energy_ + amount);
}

void Character::LoseHealth(int amount) {
  health_lost_ += amount;
  if (amount > 0 && health_lost_ >= GetMaxHealth()) {
    Die();
  }
}

int Character::GetMaxHealth() const {
  int max_health = kMaxHealth;
  int percentage = 100;
  for (auto modifier : max_health_modifiers_) {
    percentage += modifier(*this);
  }
  // TODO I think there is a cap to this.. E.g. cannot lose more than 100 max
  // health via deep wound.
  return (max_health * percentage) / 100;
}

Character::MaxHealthModifierRef Character::AddMaxHealthModifier(
    std::function<int(const Character&)> modifier) {
  max_health_modifiers_.push_front(modifier);
  return std::begin(max_health_modifiers_);
}

void Character::RemoveMaxHealthModifier(
    MaxHealthModifierRef modifier_reference) {
  max_health_modifiers_.erase(modifier_reference);
}

void Character::SetAttribute(Attribute attribute, int num) {
  // TODO Make sure to not violate primary attributes.
  std::map<int, int> kAttributeCost = {{1, 1},  {2, 3},   {3, 6},   {4, 10},
                                       {5, 15}, {6, 21},  {7, 28},  {8, 37},
                                       {9, 48}, {10, 61}, {11, 77}, {12, 97}};
  int attribute_points_back = kAttributeCost[attributes_[attribute]];
  int attribute_points_spend = kAttributeCost[num];
  assert(attribute_points_ - attribute_points_spend + attribute_points_back >
         0);
  attribute_points_ =
      attribute_points_ - attribute_points_spend + attribute_points_back;
  attributes_[attribute] = num;
}

Skill* Character::SetSkill(int pos, std::unique_ptr<Skill> skill) {
  assert(pos < 8 && pos >= 0);
  skills_[pos] = std::move(skill);
  return GetSkill(pos);
}

bool Character::ReceiveWeaponDamage(int damage, Weapon::Type type,
                                    bool blockable) {
  if (blockable && WillBlockAttack(type)) {
    // TODO maybe I can have a list of functions that want to be called on
    // certain events.
    if (GetStance()) {
      GetStance()->AttackBlocked(type);
    }

    return false;
  }

  int percentage = damage * 100 / GetMaxHealth();
  AddAdrenaline(percentage);

  LoseHealth(damage);
  return true;  // TODO return false if blocked.
}

bool Character::WillBlockAttack(Weapon::Type type) {
  if (GetStance()) {
    int chance = GetStance()->BlockChance(type);
    if (RandomDecision(Chance(chance))) {
      return true;
    }
  }
  return false;
}

bool Character::WeaponAttack(Character& target, int skill_damage,
                             bool blockable) {
  bool success = target.ReceiveWeaponDamage(
      WeaponStrikeDamage(*this, target) + skill_damage, weapon_->GetType(),
      blockable);
  if (success) {
    AddAdrenaline(25);
  }
  return success;
}

void Character::AddAdrenaline(int charges) {
  std::for_each(std::begin(skills_), std::end(skills_),
                [charges](std::unique_ptr<Skill>& skill) {
                  if (skill) skill->AddAdrenaline(charges);
                });
}

void Character::RemoveOneAdrenalineStrike() {
  std::for_each(std::begin(skills_), std::end(skills_),
                [](std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAdrenaline(25);
                });
}

void Character::Die() {
  action_ = kActionDead;
  // TODO what else happens when you die? Probably lose all timed effects.
  std::for_each(std::begin(skills_), std::end(skills_),
                [](std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAllAdrenaline();
                });
  health_lost_ = GetMaxHealth();
  energy_ = 0;
}

int Character::GetAttribute(Attribute attribute) const {
  if (attributes_.count(attribute)) {
    return attributes_.at(attribute);
  }
  return 0;
}

std::ostream& operator<<(std::ostream& out, const Character& character) {
  out << character.first_profession_ << std::endl;
  out << "Lost Health: " << character.health_lost_ << std::endl;
  out << "Energy: " << character.energy_ << std::endl;
  return out;
}