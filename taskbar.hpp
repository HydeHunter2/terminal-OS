#pragma once

#include "window.hpp"
#include "icon.hpp"

class Taskbar : public Window {
  public:
    Taskbar(const Rect& rect) : Window(rect, ' ') {}

    void addIcon(Icon* icon) {
        icon->moveTo({int(_icons.size()) * (9 + 1) + 1, 0});
        _icons.push_back(icon);
        addWindow(icon);
    }

  private:
    std::vector<Icon*> _icons;
};
