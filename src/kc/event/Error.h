#pragma once

#include <exception>

#include "kc/core/ErrorBase.hpp"

DEFINE_ERROR(EventError);

DEFINE_SUB_ERROR(ListenerAlreadyRegistered, EventError);
DEFINE_SUB_ERROR(ListenerNotFound, EventError);
DEFINE_SUB_ERROR(ResultTimeout, EventError);
DEFINE_SUB_ERROR(NoTargetSpecified, EventError);
