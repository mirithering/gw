#ifndef DAMAGE_H
#define DAMAGE_H

#include <bits/stdc++.h>

#include "character.h"

int WeaponStrikeDamage(const Character& attacker, const Character& defender,
                       int bonus_damage = 0);

#endif  // DAMAGE_H