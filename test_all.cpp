#include <gtest/gtest.h>

#include <bnetcpp/auth.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>

#include "bnetcpp/exception.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

void
handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    // Traitement de la requête
    if (req.method() == http::verb::post && req.target() == "/oauth/token") {
        // Simuler une réponse JSON avec un access_token factice
        res.result(http::status::ok);
        res.set(http::field::server, "TestServer");
        res.set(http::field::content_type, "application/json");

        // Corps de la réponse avec un token fictif
        res.body() = R"({"access_token": "fictitious_token_123456", "token_type": "bearer", "expires_in": 3600})";
        res.prepare_payload();
    } else {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Not Found";
        res.prepare_payload();
    }
}

void
do_session(tcp::socket socket, ssl::context& ctx) {
    auto stream = std::make_shared<beast::ssl_stream<beast::tcp_stream>>(std::move(socket), ctx);

    // Effectuer la poignée de main SSL
    stream->async_handshake(ssl::stream_base::server, [stream](beast::error_code ec) {
        if (!ec) {
            // Lire la requête HTTP
            auto buffer = std::make_shared<beast::flat_buffer>();
            auto req = std::make_shared<http::request<http::string_body>>();
            http::async_read(*stream, *buffer, *req, [stream, buffer, req](beast::error_code ec, std::size_t) {
                if (!ec) {
                    // Créer et envoyer la réponse HTTP
                    auto res = std::make_shared<http::response<http::string_body>>();
                    handle_request(*req, *res);
                    http::async_write(*stream, *res, [stream, res](beast::error_code ec, std::size_t) {
                        if (ec) {
                            std::cerr << "Erreur lors de l'envoi de la réponse: " << ec.message() << std::endl;
                        }
                        auto _ = stream->shutdown(ec);
                    });
                }
            });
        }
    });
}

void
do_accept(tcp::acceptor& acceptor, ssl::context& ctx) {
    acceptor.async_accept([&acceptor, &ctx](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "Nouvelle connexion acceptée" << std::endl;
            do_session(std::move(socket), ctx);
        }
        // Récursivité pour accepter les connexions suivantes
        do_accept(acceptor, ctx);
    });
}

int
run_stub() {
    try {
        const int port = 8443;

        // Contexte SSL et chargement du certificat
        ssl::context ctx(ssl::context::tlsv12_server);
        ctx.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::single_dh_use);

        // Chemins vers le certificat et la clé
        ctx.use_certificate_chain_file("server.crt");
        ctx.use_private_key_file("server.key", ssl::context::pem);

        // Initialisation de Boost.Asio
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), port));

        // Gestion des signaux pour une fermeture propre
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](beast::error_code const&, int) { ioc.stop(); });

        std::cout << "Le serveur de test écoute sur le port " << port << std::endl;

        // Démarrer l'acceptation asynchrone des connexions
        do_accept(acceptor, ctx);

        // Lancer l'io_context
        ioc.run();

    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}

TEST(all, all) {
    ASSERT_THROW(bnetcpp::auth("myid", "mysecret"), bnetcpp::error::not_ok_error);

    bnetcpp::connection conn{bnetcpp::auth("myid", "mysecret")};
}
