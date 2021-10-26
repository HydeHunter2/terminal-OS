#pragma once

#include <iostream>
#include <set>

#include "taskbar.hpp"
#include "window.hpp"

class OperatingSystem : public Window {
  public:
    OperatingSystem(Size size);

    Coordinates localCoordsToGlobal(const Coordinates& coords) const override;
    Coordinates globalCoordsToLocal(const Coordinates& coords) const override;

    Taskbar* getTaskbar() const;

    void draw();
  
  private:
    Taskbar* _taskbar;
};
