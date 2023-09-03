#include "crippling_slash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/random.h"
#include "character/build.h"
#include "character/creature.h"
#include "weapon/dagger.h"
#include "weapon/sword.h"

TEST(CripplingSlashTest, InflictBleedingAndCrippled) {
  Creature attacker =
      ConstructCreature(Profession::Warrior, Sword(),
                        {{Attribute::Swordsmanship, 12}}, CripplingSlash());
  Creature defender = ConstructCreature(Profession::Assassin, Dagger());
  attacker.target_ = &defender;

  CripplingSlash* crippling_slash =
      attacker.GetBuild().GetSkill<CripplingSlash>(0);

  std::vector<Creature> empty_;

  crippling_slash->AddAdrenaline(6 * Strike);
  attacker.GetAction() = crippling_slash->Activate(attacker, empty_, empty_);
  while (attacker.GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }
  ASSERT_TRUE(defender.HasCondition(Condition::Type::Bleeding));
  ASSERT_TRUE(defender.HasCondition(Condition::Type::Crippled));
}