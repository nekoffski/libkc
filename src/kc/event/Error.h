#pragma once

#include <exception>
namespace kc::event {

struct XventError : public std::exception {};

struct ListenerAlreadyRegistered : XventError {};

struct ListenerNotFound : XventError {};

struct ResultTimeout : XventError {};

struct NoTargetSpecified : XventError {};
}
