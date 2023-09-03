#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "character/creature.h"
#include "weapon/dagger.h"
#include "weapon/flatbow.h"
#include "weapon/sword.h"

class GwTest : public ::testing::Test {
 public:
  GwTest() {}

 protected:
  std::vector<std::unique_ptr<Creature>>& team() { return team_; }
  std::vector<std::unique_ptr<Creature>>& enemies() { return enemies_; }

 private:
  std::vector<std::unique_ptr<Creature>> team_;
  std::vector<std::unique_ptr<Creature>> enemies_;
};

Creature* AddWarriorTo(std::vector<std::unique_ptr<Creature>>& group);
Creature* AddRangerTo(std::vector<std::unique_ptr<Creature>>& group);

Creature* AddAssassinTo(std::vector<std::unique_ptr<Creature>>& group);

void TickUntilIdle(Creature* creature);

#endif  // TEST_TEST_H