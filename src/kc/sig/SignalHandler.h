#pragma once

namespace kc::sig {

class SignalHandler {
public:
    virtual void onSignal(int) = 0;
};

}
