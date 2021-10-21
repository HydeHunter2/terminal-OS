#pragma once

#include "coordinates.hpp"
#include "size.hpp"

struct Rect {
  public:
    Rect(const Coordinates& coords, const Size& size);
    Rect(const Coordinates& leftUp, const Coordinates& rightDown);
    Rect(int x, int y, int w, int h);

    bool contains(const Coordinates& point) const;

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;

    Coordinates getCoords() const;
    void setCoords(const Coordinates& coords);

    Size getSize() const;
    void setSize(const Size& size);

  private:
    Coordinates _coords;
    Size _size;
};
