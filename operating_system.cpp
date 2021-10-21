#include "operating_system.hpp"

OperatingSystem::OperatingSystem(Size size) : Window(Rect({0, 0}, size), ' ') {}


Coordinates OperatingSystem::localCoordsToGlobal(const Coordinates& coords) const {
    return coords;
}
Coordinates OperatingSystem::globalCoordsToLocal(const Coordinates& coords) const {
    return coords;
}


void OperatingSystem::draw() {
    for (int y = 0; y < getHeight(); ++y) {
        for (int x = 0; x < getWidth(); ++x) {
            std::cout << getPixel({x, y});
        }
        std::cout << '\n';
    }
}
