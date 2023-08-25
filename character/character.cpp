#include "character.h"

#include <bits/stdc++.h>

#include "base/attribute.h"
#include "base/profession.h"
#include "base/random.h"
#include "damage.h"

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

Character::Character(Profession first_profession) : maxHealth_(kMaxHealth) {
  first_profession_ = first_profession;
  armor_ = std::make_unique<Armor>(first_profession);
  health_ = kMaxHealth;
  energy_ = kMaxEnergy[first_profession];

  attribute_points_ = 200;
}

void Character::Tick(int time_passed) {
  if (time_passed % 3000 == 0) {
    RechargeEnergy();
  }
}

void Character::RechargeEnergy() {
  AddEnergy(kEnergyRegeneration.at(first_profession_));
}

void Character::AddEnergy(int amount) {
  energy_ = std::min(kMaxEnergy.at(first_profession_), energy_ + amount);
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

void Character::SetSkill(int pos, std::unique_ptr<Skill> skill) {
  assert(pos < 8 && pos >= 0);
  skills_[pos] = std::move(skill);
}

bool Character::ReceiveWeaponDamage(int damage, Weapon::Type type) {
  if (WillBlockAttack(type)) {
    // TODO maybe I can have a list of functions that want to be called on
    // certain events.
    if (stance_) {
      stance_->AttackBlocked(type);
    }

    return false;
  }

  int percentage = damage * 100 / maxHealth_;
  AddAdrenaline(percentage);

  health_ -= damage;
  std::cout << name_ << " receiving " << damage
            << " damage. Remaining health: " << health_ << ".\n";
  if (health_ <= 0) {
    Die();
  }
  return true;  // TODO return false if blocked.
}

bool Character::WillBlockAttack(Weapon::Type type) {
  if (stance_) {
    int chance = stance_->BlockChance(type);
    if (RandomDecision(Chance(chance))) {
      return true;
    }
  }
  return false;
}

void Character::WeaponAttack(Character& target, int skill_damage) {
  bool success = target.ReceiveWeaponDamage(
      WeaponStrikeDamage(*this, target, skill_damage), weapon_->GetType());
  if (success) {
    AddAdrenaline(25);
  }
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
  health_ = 0;
  std::for_each(std::begin(skills_), std::end(skills_),
                [](std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAllAdrenaline();
                });
}

int Character::GetAttribute(Attribute attribute) const {
  if (attributes_.count(attribute)) {
    return attributes_.at(attribute);
  }
  return 0;
}

std::ostream& operator<<(std::ostream& out, const Character& character) {
  out << character.first_profession_ << std::endl;
  out << "Health: " << character.health_ << std::endl;
  out << "Energy: " << character.energy_ << std::endl;
  return out;
}