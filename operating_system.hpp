#pragma once

#include <iostream>
#include <set>

#include "window.hpp"

class OperatingSystem : public Window {
  public:
    OperatingSystem(Size size);

    Coordinates localCoordsToGlobal(const Coordinates& coords) const override;
    Coordinates globalCoordsToLocal(const Coordinates& coords) const override;

    void draw();
};
