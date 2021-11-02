#include "coordinates.hpp"

Coordinates::Coordinates(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}

Coordinates Coordinates::operator+(const Coordinates& coords) const {
    return {x + coords.x, y + coords.y};
}
Coordinates Coordinates::operator-(const Coordinates& coords) const {
    return {x - coords.x, y - coords.y};
}