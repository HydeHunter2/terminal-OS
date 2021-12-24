#include "operating_system.hpp"

OperatingSystem::OperatingSystem(Size size)
    : Window(Rect({0, 0}, size), ' '), _taskbar(new Taskbar(Rect({0, getHeight() - 5}, Size(getWidth(), 5)))) {
    Window::addWindow(_taskbar);
}

Coordinates OperatingSystem::localCoordsToGlobal(const Coordinates& coords) const {
    return coords;
}
Coordinates OperatingSystem::globalCoordsToLocal(const Coordinates& coords) const {
    return coords;
}
void OperatingSystem::addWindow(Window* window) {
    BorderedWindow* borderedWindow = dynamic_cast<BorderedWindow*>(window);  // without casting?
    borderedWindow->connectWithTaskbar(getTaskbar());
    Window::addWindow(borderedWindow);
}

Taskbar* OperatingSystem::getTaskbar() const {
    return _taskbar;
}

void OperatingSystem::draw() {
    for (int y = 0; y < getHeight(); ++y) {
        std::cout << "\e[" << std::to_string(y + 1) << ";0H";
        for (int x = 0; x < getWidth(); ++x) {
            _print << getPixel({x, y});
        }
    }
}
