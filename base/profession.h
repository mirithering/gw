#ifndef PROFESSION_H
#define PROFESSION_H

#include <bits/stdc++.h>

enum class Profession {
  None,
  Warrior,
  Ranger,
  Dervish,
  Assassin,
  Paragon,
  Elementalist,
  Monk,
  Mesmer,
  Necromancer,
  Ritualist
};

std::ostream& operator<<(std::ostream& out, const Profession& profession);

#endif  // PROFESSION_H