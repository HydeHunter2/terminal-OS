#include "size.hpp"


Size::Size(int w, int h) : width(w), height(h) {}

Size Size::increased(int deltaX, int deltaY) const {
    return {width + deltaX, height + deltaY};
}