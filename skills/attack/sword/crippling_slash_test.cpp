#include "crippling_slash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "entities/build.h"
#include "entities/creature.h"
#include "test/test.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

class CripplingSlashTest : public GwTest {
 public:
  void SetUp() override {
    attacker = AddWarriorTo(team());
    attacker->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    crippling_slash =
        attacker->GetBuild().AddSkill(std::make_unique<CripplingSlash>());

    defender = AddAssassinTo(enemies());
  }

 protected:
  Creature* attacker;
  CripplingSlash* crippling_slash;
  Creature* defender;
};

TEST_F(CripplingSlashTest, InflictBleedingAndCrippled) {
  crippling_slash->AddAdrenaline(6 * Strike);
  attacker->target_ = defender;

  attacker->UseSkill(crippling_slash, world());
  AwaitIdle(attacker);
  ASSERT_TRUE(defender->HasCondition(Condition::Type::Bleeding));
  ASSERT_TRUE(defender->HasCondition(Condition::Type::Crippled));
}