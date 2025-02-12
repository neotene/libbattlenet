#include <boost/asio.hpp>
#include <memory>

namespace asio = boost::asio;
#include "client.hpp"

namespace bnetcpp {
struct client::pimpl {
    asio::io_context io_context_;
};

client::client()
    : impl_(std::make_unique<pimpl, void (*)(pimpl*)>())
{}

}   // namespace bnetcpp
