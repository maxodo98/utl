#include "gtest/gtest.h"

#include "utl/init_from.h"

TEST(init_from, init_from) {
  struct a {
    int& i_;
    float& j_;
    int& k_;
  };

  struct b {
    double z_;
    std::unique_ptr<float> x_;
    int y_;
  };

  auto src = b{0.0, std::make_unique<float>(10.0F), 42};
  auto tgt = utl::init_from<a>(src);
  ASSERT_TRUE(tgt.has_value());

  EXPECT_EQ(42, tgt->i_);
  EXPECT_EQ(10.0F, tgt->j_);
  EXPECT_EQ(42, tgt->k_);

  tgt->i_ = 7.0F;
  tgt->j_ = 77.0F;
  EXPECT_EQ(77.0F, *src.x_);
  EXPECT_EQ(7.0F, src.y_);

  tgt->k_ = 99;
  EXPECT_EQ(99, src.y_);

  auto no = utl::init_from<a>(b{});
  EXPECT_FALSE(no.has_value());
};