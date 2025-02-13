#pragma once

#include <expected>
#include <map>
#include <string>

#include "config.hpp"

namespace bnetcpp {

class BNETCPP_API client
{
   private:
    void* impl_;

   public:
    client(void* impl);
};

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
