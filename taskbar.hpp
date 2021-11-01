#pragma once

#include "window.hpp"
#include "icon.hpp"

class Taskbar : public Window {
  public:
    Taskbar(const Rect& rect);

    void addIcon(Icon* icon);

  private:
    std::vector<Icon*> _icons;
};
