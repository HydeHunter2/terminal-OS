#include "icon.hpp"

Icon::Icon(Window* relatedWindow, const Size& imageSize, std::initializer_list<char> data)
    : Window(Rect({0, 0}, imageSize)), _relatedWindow(relatedWindow), _image(imageSize, data) {}


char Icon::getPixel(const Coordinates& coords) const {
    return _image.getPixel(coords);
}

void Icon::processMouseEvent(const MouseEvent& mouseEvent) {
    if (mouseEvent.isClick() && isCliked(this, mouseEvent.getCoords())) {
        _relatedWindow->show();
        _relatedWindow->moveToFront();
    }
}
