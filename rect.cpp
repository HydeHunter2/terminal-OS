#include "rect.hpp"

Rect::Rect(const Coordinates& coords, const Size& size) : _coords(coords), _size(size) {}
Rect::Rect(const Coordinates& leftUp, const Coordinates& rightDown) : _coords(leftUp), _size({rightDown.x - leftUp.x, rightDown.y - leftUp.y}) {}
Rect::Rect(int x, int y, int w, int h) : Rect(Coordinates(x, y), Size(w, h)) {}

bool Rect::contains(const Coordinates& point) const {
    return (point.x >= _coords.x) &&
           (point.y >= _coords.y) &&
           (point.x < _coords.x + _size.width) &&
           (point.y < _coords.y + _size.height);
}

int Rect::getX() const {
    return _coords.x;
}
int Rect::getY() const {
    return _coords.y;
}
int Rect::getWidth() const {
    return _size.width;
}
int Rect::getHeight() const {
    return _size.height;
}

Coordinates Rect::getCoords() const {
    return _coords;
}
void Rect::setCoords(const Coordinates& coords) {
    _coords = coords;
}

Size Rect::getSize() const {
    return _size;
}
void Rect::setSize(const Size& size) {
    _size = size;
}
