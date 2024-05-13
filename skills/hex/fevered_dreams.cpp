#include "fevered_dreams.h"

#include "character/character.h"
#include "character/hex.h"
#include "character/skill.h"
#include "conditions/dazed.h"

namespace {

const double kDurationBase = 10;
const double kDurationIncrease = 1;

const double kDazeDurationBase = 1;
const double kDazeDurationIncrease = 2.0 / 15;

class FeveredDreamsHex : public Hex {
 public:
  void AddModifiers(Character& character) override {
    reference_ = character.callbacks_add_condition_.AddFunction(
        [](const Character& character, const Effect<Condition>& condition,
           World& world) {
          // TODO shiiit I have no idea how to copy conditions.
        });
  }

  Type GetType() const override { return Type::FeveredDreams; }

 private:
  FunctionList<void(const Character& character,
                    const Effect<Condition>& condition,
                    World& world)>::UniqueReference reference_;
};

}  // namespace

void FeveredDreams::ActivationEnd(Character& character, World& world) {
  // TODO looks like this can be generic, and the skill just gives the two
  // numbers, at 0 and at 15. But first test the actual calculation...
  Time time = (kDurationBase +
               character.GetBuild().GetAttribute(Attribute::IllusionMagic) *
                   kDurationIncrease) *
              Second;

  target_->AddHex(Effect<Hex>(time, std::make_unique<FeveredDreamsHex>()));

  if (target_->ConditionsCount() >= 2) {
    Time dazed_time = (kDazeDurationBase + character.GetBuild().GetAttribute(
                                               Attribute::IllusionMagic) *
                                               kDazeDurationIncrease) *
                      Second;
    target_->AddCondition(
        Effect<Condition>(dazed_time, std::make_unique<Dazed>()));
  }

  Skill::ActivationEnd(character, world);
}