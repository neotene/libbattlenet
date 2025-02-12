#pragma once

#include <memory>

namespace bnetcpp {

class client
{
   private:
    struct pimpl;
    std::unique_ptr<pimpl> impl_;

   public:
    client();
};

}   // namespace bnetcpp
