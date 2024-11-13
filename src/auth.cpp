#include <base64/base64.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json/src.hpp>
#include <iostream>

#include "bnetcpp/auth.hpp"
#include "boost/beast/version.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

namespace bnetcpp {
BnetConn
auth(const std::string& client_id, const std::string& client_secret)
{
    try
    {
        // Créez le contexte SSL et le socket
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();

        // Résolution de l'hôte et connexion
        tcp::resolver resolver(ioc);
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);
        if (!SSL_set_tlsext_host_name(stream.native_handle(), "us.battle.net"))
        {
            beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        auto const results = resolver.resolve("us.battle.net", "https");
        beast::get_lowest_layer(stream).connect(results);
        stream.handshake(ssl::stream_base::client);

        // Création de la requête HTTP POST
        http::request<http::string_body> req{http::verb::post, "/oauth/token", 11};
        req.set(http::field::host, "us.battle.net");
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::content_type, "application/x-www-form-urlencoded");

        // Créer le corps de la requête
        std::string body = "grant_type=client_credentials";
        req.body() = body;
        req.prepare_payload();

        // Authentification en utilisant client_id et client_secret
        std::string auth = client_id + ":" + client_secret;
        // std::string auth_base64 = beast::detail::base64_encode(auth);
        std::string auth_base64 = base64_encode(auth);
        req.set(http::field::authorization, "Basic " + auth_base64);

        // Envoie de la requête et réception de la réponse
        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);

        // Affiche le contenu de la réponse
        std::cout << res << std::endl;

        // Extraction du token d'accès
        std::string access_token;
        if (res.result() == http::status::ok)
        {
            auto json_response = boost::json::parse(beast::buffers_to_string(res.body().data()));
            access_token = json_response.at("access_token").as_string().c_str();
            std::cout << "Access Token: " << access_token << std::endl;
        } else
        {
            std::cerr << "Erreur d'authentification : " << res.result_int() << std::endl;
        }

        // Fermeture de la connexion
        beast::error_code ec;
        auto _ = stream.shutdown(ec);
        if (ec == net::error::eof)
        {
            ec = {};
        }
        if (ec)
        {
            throw beast::system_error{ec};
        }
        return BnetConn{access_token};
    } catch (const std::exception& e)
    {
        std::cerr << "Erreur : " << e.what() << std::endl;
        throw e;
    }
}

}   // namespace bnetcpp