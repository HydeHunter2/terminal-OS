#pragma once

#include "coordinates.hpp"

struct MouseEvent {
  public:
    enum Type {
        Click,
        Unclick,
        Drag,
        ScrollUp,
        ScrollDown
    };

    MouseEvent(Coordinates coordinates, Type eventType);

    Coordinates getCoords() const;
    bool isClick() const;
    bool isUnclick() const;
    bool isDrag() const;
    bool isScrollUp() const;
    bool isScrollDown() const;

  private:
    Coordinates _coords;
    Type _type;
};
