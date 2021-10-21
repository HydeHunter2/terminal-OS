#include "window.hpp"


Window::Window(const Rect& rect, char fill) : _rect(rect), _fillChar(fill) {}
Window::~Window() {}

void Window::tick() {
    for (auto const& windowToKill : _toKillQueue) {
        for (int i = _childs.size() - 1; i >= 0; --i) {
            if (windowToKill == _childs[i].get()) {
                _childs.erase(_childs.begin() + i);
            }
        }
    }
    _toKillQueue.clear();

    for (auto& child : _childs) {
        child->tick();
    }
}

void Window::setParent(Window* parent) {
    _parent = parent;
}
void Window::addWindow(Window* window) {
    window->setParent(this);
    _childs.insert(_childs.begin(), std::unique_ptr<Window>(window));
}
void Window::addToKillQueue(Window* window) {
    _toKillQueue.insert(window);
}
void Window::kill() {
    _parent->addToKillQueue(this);
}

Coordinates Window::localCoordsToGlobal(const Coordinates& coords) const {
    return _parent->localCoordsToGlobal(coords + getCoords());
}
Coordinates Window::globalCoordsToLocal(const Coordinates& coords) const {
    return _parent->globalCoordsToLocal(coords) - getCoords();
}

Window* Window::getChildByCoords(const Coordinates& coords) {
    for (const auto& child : _childs) {
        if (child->getRect().contains(globalCoordsToLocal(coords))) {
            return child.get();
        }
    }
    return nullptr;
}
bool Window::isCliked(Window* window, const Coordinates& coords) {
    if (_parent == nullptr) {
        Window* child = this;

        do {
            child = child->getChildByCoords(coords);
            if (child == window) {
                return true;
            }
        } while (child != nullptr);

        return false;
    } else {
        return _parent->isCliked(window, coords);
    }
}

Rect Window::getRect() const {
    return _rect;
}
Size Window::getSize() const {
    return _rect.getSize();
}
int Window::getWidth() const {
    return _rect.getWidth();
}
int Window::getHeight() const {
    return _rect.getHeight();
}
Coordinates Window::getCoords() const {
    return _rect.getCoords();
}
int Window::getX() const {
    return _rect.getX();
}
int Window::getY() const {
    return _rect.getY();
}

void Window::moveTo(Coordinates coords) {
    _rect.setCoords(coords);
}
void Window::resize(Size size) {
    _rect.setSize(size);
}

char Window::getPixel(const Coordinates& coords) const {
    for (const auto& child : _childs) {
        if (child->getRect().contains(coords)) {
            return child->getPixel(coords - child->getRect().getCoords());
        }
    }
    return _fillChar;
}

void Window::processMouseEvent(const MouseEvent& event) {
    for (const auto& child : _childs) {
        child->processMouseEvent(event);
    }
}
