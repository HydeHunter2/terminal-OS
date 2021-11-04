#include "icon.hpp"

Icon::Icon(Window* relatedWindow, Image* image)
    : Window(Rect({0, 0}, image->getSize())), _relatedWindow(relatedWindow), _image(image) {}

Pixel Icon::getPixel(const Coordinates& coords) const {
    Pixel pixel = _image->getPixel(coords);

    if (_relatedWindow->isFront()) {
        pixel.effect = Effect::Bold;
        pixel.backgroundColor = BackgroundColor::DarkGray;
    } else {
        pixel.effect = Effect::Reset;
        pixel.backgroundColor = BackgroundColor::Default;
    }

    return pixel;
}

void Icon::processMouseEvent(const MouseEvent& mouseEvent) {
    if (mouseEvent.isClick() && isCliked(mouseEvent.getCoords())) {
        _relatedWindow->show();
        _relatedWindow->moveToFront();
    }
}
