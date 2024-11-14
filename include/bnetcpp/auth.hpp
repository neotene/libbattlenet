#pragma once

#include <string>

#include "config.hpp"

namespace bnetcpp {

class connection {
   private:
    std::string token_;

   public:
    connection(std::string const& token)
        : token_(token) {
    }
};

extern BNETCPP_API connection auth(const std::string& client_id, const std::string& client_secret);

}   // namespace bnetcpp
