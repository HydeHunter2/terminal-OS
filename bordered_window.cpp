#include "bordered_window.hpp"

BorderedWindow::BorderedWindow(const Rect& rect, Taskbar* taskbar) : Window(rect) {
    // temp hardcode
    _innerWindow = new Window(Rect(Coordinates(1, 1), Size(getWidth() - 2, getHeight() - 2)), '#');
    _icon = new Icon(this, Size(9, 5), {'.', '.', '.', '.', '.', '.', '.', '.', '.',
                                        '.', '.', '.', '.', '.', '.', '.', '.', '.',
                                        '.', '.', '.', '.', '.', '.', '.', '.', '.',
                                        '.', '.', '.', '.', '.', '.', '.', '.', '.',
                                        '.', '.', '.', '.', '.', '.', '.', '.', '.'});

    addWindow(_innerWindow);

    // move to BorderedWindow::connectWithTaskbar?
    if (taskbar) {
        taskbar->addIcon(_icon);
    }
}

char BorderedWindow::getPixel(const Coordinates& coords) const {
    bool isExit = (coords.x == 1 && coords.y == 1);
    bool isHide = (coords.x == 3 && coords.y == 1);
    bool isHorizontalLine = (coords.y == 0 || coords.y == 2 || coords.y == getHeight() - 1);
    bool isVerticalLine = (coords.x == 0 || coords.x == getWidth() - 1);
    bool isSpace = coords.y == 1;

    if (isExit) {
        return 'X';
    } else if (isHide) {
        return 'H';
    } else if (isHorizontalLine){
        return '-';
    } else if (isVerticalLine) {
        return '|';
    } else if (isSpace) {
        return ' ';
    }

    return Window::getPixel(coords);
}

void BorderedWindow::processMouseEvent(const MouseEvent& mouseEvent) {
    if (!isVisible()) {
        return;
    }

    auto localCoords = globalCoordsToLocal(mouseEvent.getCoords());

    if (mouseEvent.isClick()) {
        _resizingBorders.clear();
        _dragOffset = std::nullopt;

        if (isCliked(this, mouseEvent.getCoords())) {
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
    _icon->kill();
    Window::kill();
}

void BorderedWindow::resize(Size size) {
    _innerWindow->resize(size.increased(-2, -2));
    Window::resize(size);
}
