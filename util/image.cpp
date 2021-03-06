#include "image.hpp"

Image::Image(const Size& size, const std::initializer_list<char>& data) : _size(size) {
    _data = new char[_size.area()];
    std::copy(data.begin(), data.end(), _data);
}

Image::~Image() {
    delete[] _data;
}

Size Image::getSize() const {
    return _size;
}

char Image::getPixel(const Coordinates& coords) const {
    return _data[coords.y * _size.width + coords.x];
}
