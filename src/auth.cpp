#include <base64/base64.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json/src.hpp>
#include <string>

#include "bnetcpp/auth.hpp"
#include "bnetcpp/exception.hpp"
#include "boost/beast/version.hpp"
#include "utils.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

namespace bnetcpp {
connection
auth(const std::string &client_id, const std::string &client_secret, host const &host) {
    net::io_context ioc;
    ssl::context ctx(ssl::context::tlsv12_client);

    try {
        ctx.set_default_verify_paths();
    } catch (boost::system::system_error const &e) {
        throw error::set_default_verify_paths_error(e);
    }

    tcp::resolver resolver(ioc);
    beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

    on_scope_exit guard([&] {
        beast::error_code ec;
        auto _ = stream.shutdown(ec);
    });

    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.hostname.data())) {
        throw error::ssl_set_tlsext_host_name_error();
    }

    try {
        auto const results = resolver.resolve(host.hostname, std::to_string(host.port));
        try {
            beast::get_lowest_layer(stream).connect(results);
        } catch (boost::system::system_error const &e) {
            throw error::tcp_connect_error(e);
        }

    } catch (boost::system::system_error const &e) {
        throw error::resolve_error(e);
    }

    try {
        stream.handshake(ssl::stream_base::client);
    } catch (boost::system::system_error const &e) {
        throw error::ssl_handshake_error(e);
    }

    http::request<http::string_body> req{http::verb::post, "/oauth/token", 11};
    req.set(http::field::host, host.hostname);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/x-www-form-urlencoded");

    std::string body = "grant_type=client_credentials";
    req.body() = body;
    req.prepare_payload();

    std::string auth = client_id + ":" + client_secret;
    std::string auth_base64 = base64_encode(auth);
    req.set(http::field::authorization, "Basic " + auth_base64);

    try {
        http::write(stream, req);
    } catch (boost::system::system_error const &e) {
        throw error::http_write_error(e);
    }

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;

    try {
        http::read(stream, buffer, res);
    } catch (boost::system::system_error const &e) {
        throw error::http_read_error(e);
    }

    std::string access_token;
    if (res.result() != http::status::ok) {
        throw error::not_ok_error();
    }

    boost::json::value json_response;
    try {
        json_response = boost::json::parse(beast::buffers_to_string(res.body().data()));
    } catch (boost::system::system_error const &e) {
        throw error::json_parse_error(e);
    }

    try {
        access_token = json_response.at("access_token").as_string().c_str();
    } catch (boost::system::system_error const &e) {
        throw error::json_invalid_error(e);
    }

    return connection{access_token};
}

}   // namespace bnetcpp