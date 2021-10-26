#pragma once

#include <initializer_list>

struct Size {
  public:
    Size(int w, int h);

    Size increased(int deltaX, int deltaY) const;
    int area() const;

    int width = 0;
    int height = 0;
};
