#include "bordered_window.hpp"

BorderedWindow::BorderedWindow(Window* innerWindow, const std::string& title)
    : Window(Rect(innerWindow->getCoords() - Coordinates(1, 3), innerWindow->getSize().increased(2, 4))),
      _innerWindow(innerWindow), _title(title) {
    _innerWindow->moveTo({1, 3});
    addWindow(_innerWindow);
}

void BorderedWindow::setIcon(Icon* icon) {
    _icon = icon;
}
void BorderedWindow::connectWithTaskbar(Taskbar* taskbar) {
    taskbar->addIcon(_icon);
}

Pixel BorderedWindow::getPixel(const Coordinates& coords) const {
    bool isExit = (coords.x == 1 && coords.y == 1);
    bool isHide = (coords.x == 3 && coords.y == 1);
    bool isHorizontalLine = (coords.y == 0 || coords.y == 2 || coords.y == getHeight() - 1);
    bool isVerticalLine = (coords.x == 0 || coords.x == getWidth() - 1);
    bool isSpace = coords.y == 1;

    if (isExit) {
        return {'X', Effect::Reset, ForgroundColor::LightRed, BackgroundColor::Default};
    } else if (isHide) {
        return 'H';
    } else if (isHorizontalLine){
        return '-';
    } else if (isVerticalLine) {
        return '|';
    } else if (isSpace) {
        if (coords.x >= 7 && coords.x < 7 + _title.size()) {  // refactor
            char symbol = _title[coords.x - 7];

            if (isFront()) {
                return {symbol, Effect::Bold, ForgroundColor::Default, BackgroundColor::Default};
            }
            return symbol;
        }
        return ' ';
    }

    return Window::getPixel(coords);
}

void BorderedWindow::processMouseEvent(const MouseEvent& mouseEvent) {
    if (!isVisible()) {
        return;
    }

    auto localCoords = globalCoordsToLocal(mouseEvent.getCoords());

    if (mouseEvent.isUnclick()) {
        _resizingBorders.clear();
        _dragOffset = std::nullopt;
    } else if (mouseEvent.isClick() && isCliked(mouseEvent.getCoords())) {
        if (localCoords.x == 0) {
            _resizingBorders.insert(Border::Left);
        } else if (localCoords.x == getRect().getWidth() - 1) {
            _resizingBorders.insert(Border::Right);
        } else if (localCoords.y == 1) {
            if (localCoords.x == 1) {
                kill();
            } else if (localCoords.x == 3) {
                hide();
            } else {
                _dragOffset = localCoords;
            }
        }
        if (localCoords.y == 0) {
            _resizingBorders.insert(Border::Up);
        } else if (localCoords.y == getRect().getHeight() - 1) {
            _resizingBorders.insert(Border::Down);
        }
    } else if (!_resizingBorders.empty()) {
        for (const auto& border : _resizingBorders) {
            switch (border) {
                case Left: {
                    resize(getSize().increased(-localCoords.x, 0));
                    moveTo(getCoords() + Coordinates(localCoords.x, 0));
                    break;
                }
                case Up: {
                    resize(getSize().increased(0, -localCoords.y));
                    moveTo(getCoords() + Coordinates(0, localCoords.y));
                    break;
                }
                case Right: {
                    resize({localCoords.x, getHeight()});
                    break;
                }
                case Down: {
                    resize({getWidth(), localCoords.y});
                    break;
                }
            }
        }
    } else if (_dragOffset != std::nullopt) {
        auto delta = localCoords - _dragOffset.value();

        moveTo(getCoords() + delta);
    }

    Window::processMouseEvent(mouseEvent);
}
void BorderedWindow::kill() {
    if (_icon) {  // can be BorderedWindow without _icon?
        _icon->kill();
    }
    Window::kill();
}

void BorderedWindow::resize(Size size) {
    _innerWindow->resize(size.increased(-2, -4));
    Window::resize(size);
}
