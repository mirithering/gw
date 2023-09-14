#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <bits/stdc++.h>

enum class Attribute {
  None,
  Strength,
  AxeMastery,
  HammerMastery,
  Swordsmanship,
  Tactics,
  CricalStrikes,
  DaggerMastery,
  DeadlyArts,
  ShadowArts,
  Marksmanship,
  Expertise,
  BeastMastery,
  WildernessSurvival,
  IllusionMagic,
};

// Double strike chance for DaggerMastery attribute.
const std::map<int, int> kDoubleStrikeChance = {
    {0, 0},   {1, 2},   {2, 4},   {3, 6},   {4, 8},   {5, 10},
    {6, 12},  {7, 14},  {8, 16},  {9, 18},  {10, 20}, {11, 22},
    {12, 24}, {13, 26}, {14, 28}, {15, 30}, {16, 32}, {17, 34},
    {18, 36}, {19, 38}, {20, 40}, {21, 42}};

#endif  // ATTRIBUTE_H