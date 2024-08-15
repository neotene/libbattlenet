#include <gtest/gtest.h>

#include <bnetcpp/auth.hpp>
#include <print>

TEST(all, all)
{
    ASSERT_TRUE(bnetcpp::auth("mynick"));
}
