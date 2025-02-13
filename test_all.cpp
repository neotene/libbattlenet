#include <gtest/gtest.h>

#include "bnetcpp/client.hpp"

TEST(all, all)
{
    ASSERT_NO_THROW(bnetcpp::authenticate(bnetcpp::REGION::EU, "myid", "mysecret"));
}
