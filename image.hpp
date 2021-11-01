#pragma once

#include <algorithm>

#include "coordinates.hpp"
#include "size.hpp"

// todo: correct copy and = contructor
class Image {
  public:
    Image(const Size& size, const std::initializer_list<char>& data);
    ~Image();

    Size getSize() const;
    char getPixel(const Coordinates& coords) const;

  private:
    const Size _size;
    char* _data{ nullptr };
};
