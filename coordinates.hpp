#pragma once

struct Coordinates {
  public:
    Coordinates(int x_coord, int y_coord);

    Coordinates operator+(const Coordinates& coords) const;
    Coordinates operator-(const Coordinates& coords) const ;

    int x = 0;
    int y = 0;
};
