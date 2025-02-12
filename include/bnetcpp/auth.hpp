#pragma once

#include <map>
#include <string>

#include "client.hpp"
#include "config.hpp"

namespace bnetcpp {

enum class REGION
{
    EU,
    US,
    APAC,
    CN
};

extern std::map<REGION, std::string> region_oauth_uris;

extern BNETCPP_API client authenticate(REGION&& region, std::string&& id, std::string&& secret);

}   // namespace bnetcpp
