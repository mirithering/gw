#include "gash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect.h"
#include "base/random.h"
#include "character/creature.h"
#include "character/damage.h"
#include "conditions/bleeding.h"
#include "test/test.h"
#include "weapon/sword.h"

class GashTest : public GwTest {
 public:
  void SetUp() override {
    warrior_ = AddWarriorTo(team());
    warrior_->GetBuild().SetAttribute(Attribute::Swordsmanship, 12);
    gash_ = warrior_->GetBuild().AddSkill(std::make_unique<Gash>());

    enemy_ = AddWarriorTo(enemies());
    warrior_->target_ = enemy_;
  }

 protected:
  Creature* warrior_;
  Creature* enemy_;
  Gash* gash_;
};

TEST_F(GashTest, NoActivationWithoutAdrenaline) {
  ASSERT_FALSE(gash_->CanActivate(*warrior_, world()));
}

TEST_F(GashTest, ActivationWithAdrenaline) {
  gash_->AddAdrenaline(6 * Strike);
  ASSERT_TRUE(gash_->CanActivate(*warrior_, world()));
}

TEST_F(GashTest, GashIsNormalAttackIfNotBleeding) {
  gash_->AddAdrenaline(6 * Strike);

  OverrideRandomValueForTesting(10);
  int expected_damage = WeaponStrikeDamage(*warrior_, *enemy_);

  OverrideRandomValueForTesting(10);
  warrior_->GetAction() = gash_->Activate(*warrior_, world());
  while (warrior_->GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_EQ(enemy_->GetLostHealth(), expected_damage);
  ASSERT_FALSE(enemy_->HasCondition(Condition::Type::DeepWound));
}

TEST_F(GashTest, GashHasAdditionalDamageIfBleeding) {
  constexpr int kExpectedSkillDamage = 17;
  constexpr int kExpectedBleedingDamage = 6;
  gash_->AddAdrenaline(6 * Strike);

  OverrideRandomValueForTesting(10);
  int expected_damage = WeaponStrikeDamage(*warrior_, *enemy_) +
                        kExpectedSkillDamage + kExpectedBleedingDamage;

  enemy_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::Bleeding));

  OverrideRandomValueForTesting(10);
  warrior_->GetAction() = gash_->Activate(*warrior_, world());
  while (warrior_->GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::Bleeding));
  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(enemy_->GetLostHealth(), expected_damage);
}

TEST_F(GashTest, GashIsInflictsDeepWoundIfBleeding) {
  gash_->AddAdrenaline(6 * Strike);

  enemy_->AddCondition(
      Effect<Condition>(10 * Second, std::make_unique<Bleeding>()));
  warrior_->GetAction() = gash_->Activate(*warrior_, world());
  while (warrior_->GetAction().GetType() != Action::Type::Idle) {
    Tick();
  }

  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::DeepWound));
}