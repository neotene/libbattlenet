#include <string>

#include "auth.hpp"

namespace bnetcpp {

std::map<REGION, std::string> region_oauth_uris = {
    {REGION::EU, "https://oauth.battle.net/authorize"},
    {REGION::US, "https://oauth.battle.net/authorize"},
    {REGION::APAC, "https://oauth.battle.net/authorize"},
    {REGION::CN, "https://oauth.battlenet.com.cn/authorize"},
};

bool
auth(std::string const &creds)
{
    return true;
}
}   // namespace bnetcpp
