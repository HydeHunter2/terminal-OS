#pragma once

#include <optional>
#include <string>
#include <set>

#include "taskbar.hpp"

class BorderedWindow : public Window {
  public:
    BorderedWindow(Window* innerWindow, const std::string& title);

    void setIcon(Icon* icon);
    void connectWithTaskbar(Taskbar* taskbar);

    Pixel getPixel(const Coordinates& coords) const override;
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
    std::string _title;
    std::set<Border> _resizingBorders;  // todo: use bitset with enum Border
    std::optional<Coordinates> _dragOffset = std::nullopt;
};
