#pragma once

#include <vector>
#include <memory>
#include <set>

#include "rect.hpp"
#include "mouse_event.hpp"
#include "bidirectional_list.hpp"

template<typename T>
bool operator==(const std::unique_ptr<T>& uniquePtr, const T* ptr) {
    return (uniquePtr.get() == ptr);
}

// todo: Rename to Object?
class Window {
  public:
    Window(const Rect& rect, char fill='.');
    virtual ~Window();

    virtual void tick();

    void setParent(Window* parent);
    void addWindow(Window* window);
    void addToKillQueue(Window* window);
    void kill();

    virtual Coordinates localCoordsToGlobal(const Coordinates& coords) const;
    virtual Coordinates globalCoordsToLocal(const Coordinates& coords) const;

    Window* getChildByCoords(const Coordinates& coords);
    bool isCliked(Window* window, const Coordinates& coords);

    Rect getRect() const;
    Size getSize() const;
    int getWidth() const;
    int getHeight() const;
    Coordinates getCoords() const;
    int getX() const;
    int getY() const;

    void moveTo(Coordinates coords);
    virtual void resize(Size size);

    virtual char getPixel(const Coordinates& coords) const;
    virtual void processMouseEvent(const MouseEvent& event);

  private:
    Rect _rect;
    char _fillChar;  // temp

    Window* _parent{ nullptr };
    // todo: use more convenient containers
    BidirectionalList<std::unique_ptr<Window>> _childs;
    std::set<Window*> _toKillQueue;  // Queue?
};
