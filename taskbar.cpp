#include "taskbar.hpp"

Taskbar::Taskbar(const Rect& rect) : Window(rect, ' ') {}

void Taskbar::addIcon(Icon* icon) {
    // fix
    icon->moveTo({int(_icons.size()) * (9 + 1) + 1, 0});
    _icons.push_back(icon);
    addWindow(icon);
}
