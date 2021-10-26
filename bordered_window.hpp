#pragma once

#include <optional>
#include <set>

#include "taskbar.hpp"

class BorderedWindow : public Window {
  public:
    BorderedWindow(const Rect& rect, Taskbar* taskbar = nullptr);

    char getPixel(const Coordinates& coords) const override;
    void processMouseEvent(const MouseEvent& mouseEvent) override;
    void kill() override;

    void resize(Size size) override;

  private:
    enum Border {
        Left,
        Up,
        Right,
        Down
    };

    Icon* _icon;
    Window* _innerWindow;
    std::set<Border> _resizingBorders;  // todo: use bitset with enum Border
    std::optional<Coordinates> _dragOffset = std::nullopt;
};
