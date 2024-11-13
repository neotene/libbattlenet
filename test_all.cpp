#include <gtest/gtest.h>

#include <bnetcpp/auth.hpp>
#include <print>

TEST(all, all)
{
    ASSERT_NO_THROW(bnetcpp::auth("myid", "mysecret"));
}
