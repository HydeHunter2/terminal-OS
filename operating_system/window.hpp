#pragma once

#include <vector>
#include <memory>
#include <set>

#include "rect.hpp"
#include "pixel.hpp"
#include "special_key.hpp"
#include "mouse_event.hpp"
#include "bidirectional_list.hpp"

// todo: refactor
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
    Window* getParent() const;
    virtual void addWindow(Window* window);
    void addToKillQueue(Window* window);
    virtual void kill();

    virtual Coordinates localCoordsToGlobal(const Coordinates& coords) const;
    virtual Coordinates globalCoordsToLocal(const Coordinates& coords) const;

    Window* getOS();
    const Window* getOS() const;
    Window* getChildByCoords(const Coordinates& coords) const;
    bool isCliked(const Coordinates& coords) const;
    void moveToFront();
    bool isFront() const;

    Rect getRect() const;
    Size getSize() const;
    int getWidth() const;
    int getHeight() const;
    Coordinates getCoords() const;
    int getX() const;
    int getY() const;

    void moveTo(Coordinates coords);
    virtual void resize(Size size);

    void hide();
    void show();
    bool isVisible() const;

    virtual Pixel getPixel(const Coordinates& coords) const;
    virtual void processMouseEvent(const MouseEvent& event);
    virtual void processSpecialKey(SpecialKey specialKey);
    virtual void processKey(char c);

  private:
    Rect _rect;
    bool _isVisible{ true };
    char _fillChar;  // temp

    Window* _parent{ nullptr };
    // todo: use more convenient containers
    BidirectionalList<std::unique_ptr<Window>> _childs;
    std::set<Window*> _toKillQueue;  // Queue?
};
