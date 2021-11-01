#pragma once

#include <iostream>
#include <set>

#include "application.hpp"
#include "taskbar.hpp"
#include "window.hpp"

// agreement: all applications should be wrapped in BorderedWindow.
class OperatingSystem : public Window {
  public:
    OperatingSystem(Size size);

    template<class App>
    void addWindow(const Application<App>& application) {
        Window::addWindow(application.getWindow());
    }
    void addWindow(Window* window) = delete;

    Coordinates localCoordsToGlobal(const Coordinates& coords) const override;
    Coordinates globalCoordsToLocal(const Coordinates& coords) const override;

    Taskbar* getTaskbar() const;

    void draw();
  
  private:
    Taskbar* _taskbar;
};
