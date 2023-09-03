#ifndef ARMOR_H
#define ARMOR_H

#include <bits/stdc++.h>

#include "base/profession.h"

namespace {
std::map<Profession, int> kArmorRating = {{Profession::Warrior, 80},
                                          {Profession::Assassin, 70},
                                          {Profession::Ranger, 70}};
}

struct ArmorPiece {
  int armor;
};

// TODO amor provides boni that are always present. (But I accidently put at
// least the energy bonus into character.cpp)

class Armor {
 public:
  Armor(Profession profession) : Armor(kArmorRating[profession]) {}
  Armor(int rating) { head_ = chest_ = arms_ = legs_ = feet_ = {rating}; }

  // TODO simplified to only hit chest for now. I assume all armor is the same.
  int Rating() const { return chest_.armor; }

 private:
  ArmorPiece head_;
  ArmorPiece chest_;
  ArmorPiece arms_;
  ArmorPiece legs_;
  ArmorPiece feet_;
};

#endif  // ARMOR_H