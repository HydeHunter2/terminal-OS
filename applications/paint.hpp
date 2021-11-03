#pragma once

#include <algorithm>

#include "bordered_window.hpp"

// fix resize (use not vector of vectors?)
// todo: save/open
class Paint : public Window {
  public:
    Paint(const Rect& rect);

    Image* getIconImage();

    char getPixel(const Coordinates& coords) const override;
    void processMouseEvent(const MouseEvent& event) override;
    void processKey(char c) override;
    void resize(Size size) override;

  private:
    std::unique_ptr<Image> _image{ nullptr };
    std::vector<std::vector<char>> _canvas;
    bool _isPainting{ false };

    std::string processCommand();
};
