#include "window.hpp"

Window::Window(const Rect& rect, char fill) : _rect(rect), _fillChar(fill) {}
Window::~Window() {}

void Window::tick() {
    for (const auto& windowToKill : _toKillQueue) {
        for (const auto& node : _childs) {
            auto& child = node->value;
            if (windowToKill == child.get()) {
                _childs.erase(node);
            }
        }
    }
    _toKillQueue.clear();

    for (const auto& child : _childs) {
        child->value->tick();
    }
}

void Window::setParent(Window* parent) {
    _parent = parent;
}
Window* Window::getParent() const {
    return _parent;
}
void Window::addWindow(Window* window) {
    window->setParent(this);
    _childs.pushFront(std::unique_ptr<Window>(window));
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

Window* Window::getOS() {
    Window* OS = this;
    while (OS->_parent != nullptr) {
        OS = OS->_parent;
    }
    return OS;
}
const Window* Window::getOS() const {
    const Window* OS = this;
    while (OS->_parent != nullptr) {
        OS = OS->_parent;
    }
    return OS;
}
Window* Window::getChildByCoords(const Coordinates& coords) const {
    for (const auto& node : _childs) {
        auto& child = node->value;
        if (child->getRect().contains(globalCoordsToLocal(coords))) {
            return child.get();
        }
    }
    return nullptr;
}
bool Window::isCliked(const Coordinates& coords) const {
    const Window* windowToCheck = getOS();

    while (windowToCheck != nullptr) {
        if (windowToCheck == this) {
            return true;
        }
        windowToCheck = windowToCheck->getChildByCoords(coords);
        
    }

    return false;
}
void Window::moveToFront() {
    _parent->_childs.moveToBegin(_parent->_childs.find(this));
}

bool Window::isFront() const {
    if (_parent == nullptr) {
        return true;
    }

    return _parent->_childs.front()->value == this;
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

Pixel Window::getPixel(const Coordinates& coords) const {
    for (const auto& node : _childs) {
        auto& child = node->value;
        if (child->isVisible() && child->getRect().contains(coords)) {
            return child->getPixel(coords - child->getRect().getCoords());
        }
    }
    return _fillChar;
}

void Window::hide() {
    _isVisible = false;
}
void Window::show() {
    _isVisible = true;
}
bool Window::isVisible() const {
    return _isVisible;
}

void Window::processMouseEvent(const MouseEvent& event) {
    if (!isVisible()) {
        return;
    }

    if (event.isClick()) {
        // todo: Refactor using moveToFront()
        _childs.moveToBegin(_childs.find(getChildByCoords(event.getCoords())));
    }

    for (const auto& node : _childs) {
        auto& child = node->value;
        child->processMouseEvent(event);
    }
}
void Window::processKey(char c) {
    for (const auto& node : _childs) {
        auto& child = node->value;
        child->processKey(c);
    }
}


