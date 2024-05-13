#include "fevered_dreams.h"

#include "conditions/dazed.h"
#include "entities/creature.h"
#include "entities/hex.h"
#include "entities/skill.h"

namespace {

const double kDurationBase = 10;
const double kDurationIncrease = 1;

const double kDazeDurationBase = 1;
const double kDazeDurationIncrease = 2.0 / 15;

class FeveredDreamsHex : public Hex {
 public:
  void AddModifiers(Creature& creature) override {
    reference_ = creature.callbacks_add_condition_.AddFunction(
        [](const Creature& creature,
           const EffectDeprecated<Condition>& condition, World& world) {
          // TODO shiiit I have no idea how to copy conditions.
        });
  }

  Type GetType() const override { return Type::FeveredDreams; }

 private:
  FunctionList<void(const Creature& creature,
                    const EffectDeprecated<Condition>& condition,
                    World& world)>::UniqueReference reference_;
};

}  // namespace

void FeveredDreams::ActivationEnd(Creature& creature, World& world) {
  // TODO looks like this can be generic, and the skill just gives the two
  // numbers, at 0 and at 15. But first test the actual calculation...
  Time time = (kDurationBase +
               creature.GetBuild().GetAttribute(Attribute::IllusionMagic) *
                   kDurationIncrease) *
              Second;

  target_->AddHex(
      EffectDeprecated<Hex>(time, std::make_unique<FeveredDreamsHex>()));

  if (target_->ConditionsCount() >= 2) {
    Time dazed_time = (kDazeDurationBase + creature.GetBuild().GetAttribute(
                                               Attribute::IllusionMagic) *
                                               kDazeDurationIncrease) *
                      Second;
    target_->AddCondition(
        EffectDeprecated<Condition>(dazed_time, std::make_unique<Dazed>()));
  }

  Skill::ActivationEnd(creature, world);
}