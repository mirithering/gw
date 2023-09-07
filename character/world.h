#ifndef WORLD_H
#define WORLD_H

#include <bits/stdc++.h>

class Creature;

struct World {
  std::vector<std::unique_ptr<Creature>> team;
  std::vector<std::unique_ptr<Creature>> enemies;
};

#endif