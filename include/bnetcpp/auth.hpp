#pragma once

#include <string>

#include "config.hpp"

namespace bnetcpp {

class bnet_conn
{
   private:
    std::string token_;

   public:
    bnet_conn(std::string const& token)
        : token_(token)
    {}
};

extern BNETCPP_API bnet_conn auth(const std::string& client_id, const std::string& client_secret);

}   // namespace bnetcpp
