#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <expected>
#include <iostream>
#include <map>
#include <stdexcept>

#include "boost/asio/io_context.hpp"
#include "client.hpp"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = net::ip::tcp;         // from <boost/asio/ip/tcp.hpp>

namespace bnetcpp {
struct pimpl {
    tcp::socket socket_;

    pimpl(tcp::socket&& socket)
        : socket_(std::move(socket))
    {}
};

client::client(void* impl)
    : impl_(impl)
{}

std::map<REGION, std::string> region_oauth_uris = {
    {REGION::EU, "https://oauth.battle.net/authorize"},
    {REGION::US, "https://oauth.battle.net/authorize"},
    {REGION::APAC, "https://oauth.battle.net/authorize"},
    {REGION::CN, "https://oauth.battlenet.com.cn/authorize"},
};

client
authenticate(REGION&& region, std::string&& id, std::string&& secret)
{
    try
    {
        // Le contexte d'exécution de Boost.Asio
        net::io_context ioc;

        // Les résolveurs DNS pour obtenir l'adresse IP du serveur
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Remplacez par l'URL de l'API Battle.net que vous souhaitez appeler
        auto const host = "us.api.blizzard.com";
        auto const port = "443";
        auto const target = "/data/wow/token/?namespace=dynamic-us&locale=en_US&access_token=YOUR_ACCESS_TOKEN";

        // Regardez l'adresse du serveur
        auto const results = resolver.resolve(host, port);

        // Faire la connexion
        stream.connect(results);

        // Configurer la requête HTTP
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Envoyer la requête HTTP
        http::write(stream, req);

        // Cette variable va contenir la réponse
        beast::flat_buffer buffer;

        // Déclare une réponse pour contenir la réponse du serveur
        http::response<http::dynamic_body> res;

        // Recevoir la réponse HTTP
        http::read(stream, buffer, res);

        // Afficher la réponse
        std::cout << res << std::endl;

        // Fermer la connexion
        beast::error_code ec;
        ec = stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // Graceful close
        if (ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

    } catch (std::exception const& e)
    {
        // std::cerr << "Error: " << e.what() << std::endl;
        throw std::runtime_error("1");
    }
}

}   // namespace bnetcpp
