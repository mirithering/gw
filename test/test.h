#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "character/character.h"
#include "character/world.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

class GwTest : public ::testing::Test {
 public:
  GwTest() {}

 protected:
  std::vector<std::unique_ptr<Character>>& team() { return world_.team; }
  std::vector<std::unique_ptr<Character>>& enemies() { return world_.enemies; }

  World& world() { return world_; }

 private:
  World world_;
};

Character* AddWarriorTo(std::vector<std::unique_ptr<Character>>& group);
Character* AddRangerTo(std::vector<std::unique_ptr<Character>>& group);

Character* AddAssassinTo(std::vector<std::unique_ptr<Character>>& group);
Character* AddMesmerTo(std::vector<std::unique_ptr<Character>>& group);

void AwaitIdle(Character* character);

#endif  // TEST_TEST_H