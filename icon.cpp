#include "icon.hpp"

Icon::Icon(Window* relatedWindow, Image* image)
    : Window(Rect({0, 0}, image->getSize())), _relatedWindow(relatedWindow), _image(image) {}

char Icon::getPixel(const Coordinates& coords) const {
    return _image->getPixel(coords);
}

void Icon::processMouseEvent(const MouseEvent& mouseEvent) {
    if (mouseEvent.isClick() && isCliked(mouseEvent.getCoords())) {
        _relatedWindow->show();
        _relatedWindow->moveToFront();
    }
}
