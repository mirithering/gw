#include "gash.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

#include "base/effect_deprecated.h"
#include "base/random.h"
#include "conditions/bleeding.h"
#include "entities/creature.h"
#include "entities/damage.h"
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
  warrior_->UseSkill(gash_, world());
  AwaitIdle(warrior_);

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

  enemy_->AddEffect(std::make_unique<Bleeding>(*enemy_, 10 * Second));
  ASSERT_TRUE(enemy_->HasEffect(Effect::Type::Bleeding));

  OverrideRandomValueForTesting(10);
  warrior_->UseSkill(gash_, world());
  AwaitIdle(warrior_);

  ASSERT_TRUE(enemy_->HasEffect(Effect::Type::Bleeding));
  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::DeepWound));
  ASSERT_EQ(enemy_->GetLostHealth(), expected_damage);
}

TEST_F(GashTest, GashIsInflictsDeepWoundIfBleeding) {
  gash_->AddAdrenaline(6 * Strike);

  enemy_->AddEffect(std::make_unique<Bleeding>(*enemy_, 10 * Second));
  ASSERT_TRUE(enemy_->HasEffect(Effect::Type::Bleeding));

  warrior_->UseSkill(gash_, world());
  AwaitIdle(warrior_);

  ASSERT_TRUE(enemy_->HasCondition(Condition::Type::DeepWound));
}