#include "build.h"

#include <bits/stdc++.h>
#include <gtest/gtest.h>

TEST(BuildTest, GetAttributeZeroIfUnset) {
  Build build;
  ASSERT_EQ(build.GetAttribute(Attribute::AxeMastery), 0);
}

TEST(BuildTest, SetAttributeSetsAttribute) {
  Build build;
  build.SetAttribute(Attribute::Swordsmanship, 12);
  ASSERT_EQ(build.GetAttribute(Attribute::Swordsmanship), 12);
}