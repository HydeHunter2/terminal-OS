#pragma once

#include "window.hpp"
#include "image.hpp"
#include "rect.hpp"

class Icon : public Window {
  public:
    Icon(Window* relatedWindow, Image* image);

    Pixel getPixel(const Coordinates& coords) const override;
    void processMouseEvent(const MouseEvent& mouseEvent) override;

  private:
    Window* _relatedWindow{ nullptr };
    Image* _image;
};
