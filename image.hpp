#pragma once

#include <algorithm>

#include "coordinates.hpp"
#include "size.hpp"

class Image {
  public:
    Image(const Size& size, std::initializer_list<char> data);
    ~Image();

    char getPixel(const Coordinates& coords) const;

  private:
    const Size _size;
    char* _data{ nullptr };
};
