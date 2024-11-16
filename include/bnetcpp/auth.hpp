#pragma once

#include <winsock2.h>

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

struct host {
    char const* const hostname;
    unsigned short const port;
};

const host bnet_europe = {"eu.battle.net", 443};

extern BNETCPP_API connection auth(const std::string& client_id, const std::string& client_secret,
                                   host const& host = bnet_europe);

}   // namespace bnetcpp
