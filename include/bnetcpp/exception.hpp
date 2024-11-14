#pragma once

#include <boost/system.hpp>
#include <exception>
#include <format>

#include "boost/system/system_error.hpp"

namespace bnetcpp {
namespace error {

class exception : public std::exception {
   private:
    std::string what_;

   public:
    exception(std::string const& what)
        : what_(what) {
    }

    virtual const char* what() const override {
        return what_.data();
    }
};

#define BNETCPP_DECL_ERROR(name, what_msg) \
    class name : public exception {        \
       public:                             \
        name()                             \
            : exception(what_msg) {        \
        }                                  \
    }

#define BNETCPP_DECL_PARAM_ERROR(name, param_error_type, what_msg)          \
    class name : public exception {                                         \
       public:                                                              \
        name(param_error_type const& param_error)                           \
            : exception(std::format(what_msg ": {}", param_error.what())) { \
        }                                                                   \
    }

BNETCPP_DECL_PARAM_ERROR(set_default_verify_paths_error, boost::system::system_error,
                         "set_default_verify_paths_error error");

BNETCPP_DECL_ERROR(ssl_set_tlsext_host_name_error, " error");

BNETCPP_DECL_PARAM_ERROR(tcp_connect_error, boost::system::system_error, "tcp_connect_error error");
BNETCPP_DECL_PARAM_ERROR(resolve_error, boost::system::system_error, "resolve_error error");
BNETCPP_DECL_PARAM_ERROR(ssl_handshake_error, boost::system::system_error, "ssl_handshake_error error");
BNETCPP_DECL_PARAM_ERROR(http_write_error, boost::system::system_error, "http_write_error error");
BNETCPP_DECL_PARAM_ERROR(http_read_error, boost::system::system_error, "http_read_error error");

BNETCPP_DECL_ERROR(not_ok_error, "not_ok_error");

BNETCPP_DECL_PARAM_ERROR(json_parse_error, boost::system::system_error, "json_parse_error error");
BNETCPP_DECL_PARAM_ERROR(json_invalid_error, boost::system::system_error, "json_invalid_error error");

}   // namespace error
}   // namespace bnetcpp