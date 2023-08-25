#include "weapon.h"

bool IsMeele(Weapon::Type type) {
  switch (type) {
    case Weapon::Type::Axe:
    case Weapon::Type::Sword:
    case Weapon::Type::Dagger:
    case Weapon::Type::Hammer:
    case Weapon::Type::Scythe:
      return true;
    default:
      return false;
  }
}

bool IsProjectile(Weapon::Type type) {
  switch (type) {
    case Weapon::Type::Spear:
    case Weapon::Type::Bow:
      return true;
    default:
      return false;
  }
}