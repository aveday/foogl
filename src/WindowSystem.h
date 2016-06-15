#pragma once
#include "components.h"

class WindowSystem {
public:
    void run();

private:
    void MakeWindow(Window &window);
    bool HasResized(Window &window);
};

