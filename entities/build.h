#ifndef CHARACTER_BUILD_H
#define CHARACTER_BUILD_H

#include <bits/stdc++.h>

#include "armor/armor.h"
#include "base/profession.h"
#include "skill.h"
#include "weapon/weapon.h"

class Build {
 public:
  Build() = default;
  Build(const Build&) = delete;
  Build& operator=(const Build&) = delete;

  void SetFirstProfession(Profession profession) {
    std::map<Profession, int> max_energy_per_profession = {
        {Profession::Warrior, 20},     {Profession::Ranger, 25},
        {Profession::Dervish, 25},     {Profession::Assassin, 25},
        {Profession::Paragon, 30},     {Profession::Elementalist, 30},
        {Profession::Monk, 30},        {Profession::Mesmer, 30},
        {Profession::Necromancer, 30}, {Profession::Ritualist, 30}};

    SetMaxEnergy(max_energy_per_profession[profession]);
    first_profession_ = profession;
  }
  Profession GetFirstProfession() const { return first_profession_; }

  void SetSecondProfession(Profession profession) {
    second_profession_ = profession;
  }
  Profession GetSecondProfession() const { return second_profession_; }

  void SetWeapon(std::unique_ptr<Weapon> weapon) {
    weapon_ = std::move(weapon);
  }
  const Weapon& GetWeapon() const { return *weapon_.get(); }

  void SetArmor(std::unique_ptr<Armor> armor) { armor_ = std::move(armor); }
  const Armor* GetArmor() const { return armor_.get(); }

  void SetMaxHealth(int health) { max_health_ = health; }
  int GetMaxHealth() const { return max_health_; }

  void SetMaxEnergy(int energy) { max_energy_ = energy; }
  int GetMaxEnergy() const { return max_energy_; }

  void SetAttribute(Attribute attribute, int num) {
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
  int GetAttribute(Attribute attribute) const {
    if (attributes_.count(attribute)) {
      return attributes_.at(attribute);
    }
    return 0;
  }

  template <class... S>
  void SetSkills(std::unique_ptr<S>... skills) {
    (skills_.push_back(std::move(skills)), ...);
  }
  template <class S>
  S* AddSkill(std::unique_ptr<S> skill) {
    assert(skills_.size() < 8);
    skills_.push_back(std::move(skill));
    return GetSkill<S>(skills_.size() - 1);
  }
  template <class S>
  S* GetSkill(unsigned int pos) const {
    assert(pos < skills_.size() && pos >= 0);
    assert(skills_[pos]);
    return static_cast<S*>(skills_[pos].get());
  }
  const std::vector<std::unique_ptr<Skill>>& GetSkills() const {
    return skills_;
  }

 private:
  Profession first_profession_ = Profession::None;
  Profession second_profession_ = Profession::None;

  std::unique_ptr<Weapon> weapon_;
  std::unique_ptr<Armor> armor_;

  std::map<Attribute, int> attributes_;
  // Note that Skills themselves are modifiable even in const builds. This is on
  // purpose because skills recharge etc.
  // TODO when adding skills, make sure to only allow skills of the correct
  // class and only one elite attack.

  std::vector<std::unique_ptr<Skill>> skills_;

  int attribute_points_ = 200;

  int max_health_ = 480;
  int max_energy_ = 0;
};

template <class W, class... S>
std::unique_ptr<Build> ConstructBuild(Profession first_profession,
                                      std::unique_ptr<W> weapon,
                                      std::map<Attribute, int> attributes = {},
                                      std::unique_ptr<S>... skills) {
  std::unique_ptr<Build> build = std::make_unique<Build>();
  build->SetFirstProfession(first_profession);
  build->SetWeapon(std::move(weapon));
  build->SetArmor(std::make_unique<Armor>(first_profession));
  for (std::pair<Attribute, int> attribute : attributes) {
    build->SetAttribute(attribute.first, attribute.second);
  }
  build->SetSkills(std::move(skills)...);
  return build;
}

#endif  // CHARACTER_BUILD_H