#pragma once

#include <string>

#include "config.hpp"

namespace bnetcpp {

class BnetConn
{
   private:
    std::string token_;

   public:
    BnetConn(std::string const& token)
        : token_(token)
    {}
};

extern BNETCPP_API BnetConn auth(const std::string& client_id, const std::string& client_secret);

}   // namespace bnetcpp
