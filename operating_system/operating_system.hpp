#pragma once

#include <iostream>
#include <set>

#include "colorful_print.hpp"
#include "application.hpp"
#include "taskbar.hpp"
#include "window.hpp"

// agreement: all applications should be wrapped in BorderedWindow.
class OperatingSystem : public Window {
  public:
    OperatingSystem(Size size);

    Coordinates localCoordsToGlobal(const Coordinates& coords) const override;
    Coordinates globalCoordsToLocal(const Coordinates& coords) const override;
    void addWindow(Window* window) override;

    Taskbar* getTaskbar() const;

    void draw();
  
  private:
    Taskbar* _taskbar;
    ColorfulPrint _print;
};
