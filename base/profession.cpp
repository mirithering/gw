#include "profession.h"

#include <bits/stdc++.h>

std::ostream& operator<<(std::ostream& out, const Profession& profession) {
  switch (profession) {
    case Profession::None:
      out << "None";
      break;
    case Profession::Warrior:
      out << "Warrior";
      break;
    case Profession::Ranger:
      out << "Ranger";
      break;
    case Profession::Dervish:
      out << "Dervish";
      break;
    case Profession::Assassin:
      out << "Assassin";
      break;
    case Profession::Paragon:
      out << "Paragon";
      break;
    case Profession::Elementalist:
      out << "Elementalist";
      break;
    case Profession::Monk:
      out << "Monk";
      break;
    case Profession::Mesmer:
      out << "Mesmer";
      break;
    case Profession::Necromancer:
      out << "Necromancer";
      break;
    case Profession::Ritualist:
      out << "Ritualist";
      break;
  }
  return out;
}