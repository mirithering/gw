#include "edge_of_extinction.h"

#include <bits/stdc++.h>

#include "armor/armor.h"
#include "entities/creature.h"

class EdgeOfExtinctionActor : public TimedObject {
 public:
  EdgeOfExtinctionActor(Creature* spirit, World* world)
      : spirit_(spirit), world_(world) {}

  ~EdgeOfExtinctionActor() { spirit_->Die(); }

  void Tick(Time time_passed) override {
    // TODO kill spirit if timed out, but does this belong here?
    if (spirit_->GetActionType() == Action::Type::Idle) {
      // Find all creatures in range of the spirit, and give them the spirit's
      // effect.
      // TODO sounds like this is common to all passive spirits.

      std::vector<Creature*> creatures_in_spirit_range;
      for (Creature* creature : world_->AllCreatures()) {
        if (InRange(creature->GetPosition(), spirit_->GetPosition(),
                    SpiritRange)) {
          creatures_in_spirit_range.push_back(creature);
        }
      }

      for (Creature* creature : creatures_in_spirit_range) {
        FunctionList<void(const Creature&)>::UniqueReference
            callback_reference =
                creature->callbacks_death_.AddFunction([&](const Creature&) {
                  for (Creature* other_creature : creatures_in_spirit_range) {
                    if (other_creature == creature ||
                        other_creature->type_ != creature->type_) {
                      continue;
                    }
                    // Only lose health if below 90%
                    if (other_creature->GetLostHealth() <=
                        0.1 * other_creature->GetMaxHealth()) {
                      continue;
                    }
                    // TODO calculate damage based on attribute level
                    int damage = 50;
                    other_creature->LoseHealth(damage);
                  }
                });
        // EffectDeprecated lasts for one tick, it will be renewed every round
        // as long as we stay in range..
        // TODO stuck here.. this can't be created, the whole effect logic is
        // overly complicated.
        creature->AddEffectDeprecated(
            EffectDeprecated<
                FunctionList<void(const Creature&)>::UniqueReference>(
                Time(1),
                std::make_unique<
                    FunctionList<void(const Creature&)>::UniqueReference>(
                    std::move(callback_reference))));
      }
    }
  }

 private:
  Creature* spirit_;  // TODO what if the spirit goes away...
  World* world_;
};

void EdgeOfExtinction::ActivationEnd(Creature& creature, World& world) {
  // TODO calculate spirit level based on attribute.
  int spirit_level = 8;

  std::unique_ptr<Build> spirit_build;
  // TODO spirits always have similar armor and health, unify this.
  spirit_build->SetArmor(std::make_unique<Armor>(
      6 * spirit_level));  // TODO this is only normal mode, different armor
                           // in hard mode
  spirit_build->SetMaxHealth(
      20 * spirit_level);  // TODO modified by creature's spawning power

  std::unique_ptr<Creature> spirit =
      make_unique<Creature>(std::move(spirit_build), creature.GetPosition());

  spirit_actor_ = std::make_unique<EdgeOfExtinctionActor>(spirit.get(), &world);
  world.AlliesOf(creature).push_back(std::move(spirit));
}