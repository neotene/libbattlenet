#include <gtest/gtest.h>

#include <bnetcpp/auth.hpp>

#include "bnetcpp/exception.hpp"

TEST(all, all) {
    ASSERT_THROW(bnetcpp::auth("myid", "mysecret"), bnetcpp::error::not_ok_error);
}
