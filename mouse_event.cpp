#include "mouse_event.hpp"

MouseEvent::MouseEvent(Coordinates coordinates, Type eventType) : _coords(coordinates), _type(eventType) {}

Coordinates MouseEvent::getCoords() const {
    return _coords;
}
bool MouseEvent::isClick() const {
    return _type == Click;
}
bool MouseEvent::isUnclick() const {
    return _type == Unclick;
}
bool MouseEvent::isDrag() const {
    return _type == Drag;
}
