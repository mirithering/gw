#ifndef WORLD_H
#define WORLD_H

#include <bits/stdc++.h>

#include "base/logging.h"

class Character;

struct World {
  std::vector<std::unique_ptr<Character>> team;
  std::vector<std::unique_ptr<Character>> enemies;

  std::vector<std::unique_ptr<Character>>& EnemiesOf(
      const Character& character) {
    for (const auto& other : team) {
      if (other.get() == &character) {
        return enemies;
      }
    }
    for (const auto& other : enemies) {
      if (other.get() == &character) {
        return team;
      }
    }
    assert(false);
  }
};

#endif