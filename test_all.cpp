#include <gtest/gtest.h>

#include "bnetcpp/auth.hpp"

TEST(all, all)
{
    ASSERT_TRUE(bnetcpp::authenticate(bnetcpp::REGION::EU, "myid", "mysecret"));
}
