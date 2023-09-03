#ifndef ENGINE_H
#define ENGINE_H

#include <bits/stdc++.h>

class Creature;

void NextActions(std::vector<std::unique_ptr<Creature>>& team,
                 std::vector<std::unique_ptr<Creature>>& enemies);

#endif  // ENGINE_H