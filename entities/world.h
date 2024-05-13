#ifndef WORLD_H
#define WORLD_H

#include <bits/stdc++.h>

#include "base/logging.h"

class Creature;
class Creature;

struct World {
  std::vector<std::unique_ptr<Creature>> team;
  std::vector<std::unique_ptr<Creature>> team_allies;

  std::vector<std::unique_ptr<Creature>> enemies;
  std::vector<std::unique_ptr<Creature>> enemy_allies;

  std::vector<std::unique_ptr<Creature>>& EnemiesOf(const Creature& creature) {
    for (const auto& other : team) {
      if (other.get() == &creature) {
        return enemies;
      }
    }
    for (const auto& other : enemies) {
      if (other.get() == &creature) {
        return team;
      }
    }
    assert(false);
  }
};

#endif