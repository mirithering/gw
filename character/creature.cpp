#include "creature.h"

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

Creature::Creature(std::unique_ptr<Build> build) : build_(std::move(build)) {
  health_lost_ = 0;
  energy_ = kMaxEnergy[build_->GetFirstProfession()];
}

void Creature::Tick(Time time_passed) {
  if (time_passed % (3 * Second) == Time(0)) {
    EnergyGeneration();
  }
  if (time_passed % Second == Time(0)) {
    HealthGeneration();
  }

  if (GetAction().Tick() == Action::Result::End) {
    GetAction() = kActionIdle;
  }

  // TODO Remove ended conditions.
}

void Creature::HealthGeneration() {
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

void Creature::EnergyGeneration() {
  int energy_generation = kEnergyRegeneration.at(build_->GetFirstProfession());
  AddEnergy(energy_generation);
}

void Creature::AddEnergy(int amount) {
  energy_ =
      std::min(kMaxEnergy.at(build_->GetFirstProfession()), energy_ + amount);
}

void Creature::LoseHealth(int amount) {
  if (amount > 0)
    std::cout << name_ << " losing " << amount << " health." << std::endl;
  health_lost_ += amount;
  if (amount > 0 && health_lost_ >= GetMaxHealth()) {
    Die();
  }
}

int Creature::GetMaxHealth() const {
  int max_health = kMaxHealth;
  int percentage = 100;
  for (auto modifier : max_health_modifiers_) {
    percentage += modifier(*this);
  }
  // TODO I think there is a cap to this.. E.g. cannot lose more than 100 max
  // health via deep wound.
  return (max_health * percentage) / 100;
}

Creature::MaxHealthModifierRef Creature::AddMaxHealthModifier(
    std::function<int(const Creature&)> modifier) {
  max_health_modifiers_.push_front(modifier);
  return std::begin(max_health_modifiers_);
}

void Creature::RemoveMaxHealthModifier(
    MaxHealthModifierRef modifier_reference) {
  max_health_modifiers_.erase(modifier_reference);
}

bool Creature::ReceiveWeaponDamage(int damage, Weapon::Type type,
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

bool Creature::WillBlockAttack(Weapon::Type type) {
  if (GetStance()) {
    int chance = GetStance()->BlockChance(type);
    if (RandomDecision(Chance(chance))) {
      return true;
    }
  }
  return false;
}

bool Creature::WeaponAttack(Creature& target, int skill_damage,
                            bool blockable) {
  bool success = target.ReceiveWeaponDamage(
      WeaponStrikeDamage(*this, target) + skill_damage,
      build_->GetWeapon().GetType(), blockable);
  if (success) {
    AddAdrenaline(25);
  }
  return success;
}

void Creature::AddAdrenaline(int charges) {
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [charges](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->AddAdrenaline(charges);
                });
}

void Creature::RemoveOneAdrenalineStrike() {
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAdrenaline(25);
                });
}

void Creature::Die() {
  action_ = kActionDead;
  // TODO what else happens when you die? Probably lose all timed effects.
  const auto& skills = build_->GetSkills();
  std::for_each(std::begin(skills), std::end(skills),
                [](const std::unique_ptr<Skill>& skill) {
                  if (skill) skill->LoseAllAdrenaline();
                });
  health_lost_ = GetMaxHealth();
  energy_ = 0;
}
