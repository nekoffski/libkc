#pragma once

#include <csignal>

#include "SignalHandler.h"

namespace kc::sig {

void setupSignalHandler(SignalHandler*);
void resetSignalHandler();

}  // namespace kc::sig