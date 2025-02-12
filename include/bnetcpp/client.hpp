#pragma once

#include <memory>

namespace bnetcpp {

class client
{
   private:
    struct impl;
    std::unique_ptr<impl> impl_;
};

}   // namespace bnetcpp
