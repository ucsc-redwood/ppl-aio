#include <gtest/gtest.h>

#include <glm/glm.hpp>

void test_glm() {
  glm::vec3 v1(1.0f, 2.0f, 3.0f);
  glm::vec3 v2(4.0f, 5.0f, 6.0f);
  glm::vec3 v3 = v1 + v2;
  EXPECT_EQ(v3.x, 5.0f);
  EXPECT_EQ(v3.y, 7.0f);
  EXPECT_EQ(v3.z, 9.0f);
}

TEST(TestGlm, TestGlm) { test_glm(); }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  const auto ret = RUN_ALL_TESTS();
  return ret;
}
