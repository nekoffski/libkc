#include "Signal.h"

#include "kc/core/Log.h"

static kc::sig::SignalHandler* signalHandler = nullptr;

static inline void signalHandlerWrapper(int signal) {
    ASSERT(signalHandler != nullptr, "Signal handler is not set up");
    LOG_INFO("received signal: {}", signal);

    signalHandler->onSignal(signal);
}

namespace kc::sig {

void setupSignalHandler(SignalHandler* sigHandler) {
    LOG_TRACE("Setting up signal handler");
    ASSERT(signalHandler == nullptr, "Could not set signal handler twice");

    signalHandler = sigHandler;

    std::signal(SIGINT, signalHandlerWrapper);
    std::signal(SIGTERM, signalHandlerWrapper);

    LOG_INFO("Signal handler registered successfully");
}

}  // namespace kc::sig
