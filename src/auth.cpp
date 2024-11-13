#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/ssl/ssl_stream.hpp>
#include <boost/beast/version.hpp>
#include <boost/system/error_code.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace ssl = boost::asio::ssl;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

class BnetConn
{
   private:
    std::string token_;
};

BnetConn
auth(const std::string& client_id, const std::string& client_secret)
{
    try
    {
        // Définir le serveur et le port
        const std::string host = "us.battle.net";
        const std::string port = "443";   // HTTPS

        // Contexte I/O
        asio::io_context ioc;

        // Resolver l'hôte
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(host, port);

        // Créer le socket
        beast::ssl_stream<beast::tcp_stream> stream(ioc, ssl::context::tlsv12_client{});

        // Connexion au serveur
        asio::connect(stream.next_layer(), results.begin(), results.end());

        // Effectuer la poignée de main SSL
        stream.handshake(ssl::stream_base::client);

        // Construire la requête pour obtenir le jeton d'accès
        http::request<http::string_body> req{http::verb::post, "/oauth/token", 11};
        req.set(http::field::host, host);
        req.set(http::field::content_type, "application/x-www-form-urlencoded");
        req.set(http::field::accept, "application/json");

        // Corps de la requête (envoi des paramètres client_id et client_secret)
        req.body() = "grant_type=client_credentials&client_id=" + client_id + "&client_secret=" + client_secret;
        req.prepare_payload();

        // Envoyer la requête
        http::write(stream, req);

        // Lire la réponse
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);

        // Vérifier le statut
        if (res.result() == http::status::ok)
        {
            std::cout << "Réponse de Battle.net: " << res.body() << std::endl;
        } else
        {
            std::cerr << "Erreur: " << res.result() << " - " << res.reason() << std::endl;
        }

        // Fermer la connexion SSL
        beast::error_code ec;
        stream.shutdown(ec);

    } catch (const std::exception& e)
    {
        std::cerr << "Erreur: " << e.what() << std::endl;
    }
}