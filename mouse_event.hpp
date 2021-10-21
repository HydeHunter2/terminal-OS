#pragma once

#include "coordinates.hpp"

struct MouseEvent {
  public:
    enum Type {
        Click,
        Drag
    };

    MouseEvent(Coordinates coordinates, Type eventType);

    Coordinates getCoords() const;
    bool isClick() const;
    bool isDrag() const;

  private:
    Coordinates _coords;
    Type _type;
};
