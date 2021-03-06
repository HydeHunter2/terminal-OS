#include "paint.hpp"

Paint::Paint(const Rect& rect) : Window(rect) {
    _image = std::unique_ptr<Image>(new Image(Size(9, 5),
        {'-', '-', '-', '-', '-', '-', '-', '-', '-',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '|', ' ', 'P', 'a', 'i', 'n', 't', ' ', '|',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '-', '-', '-', '-', '-', '-', '-', '-', '-'}));

    _canvas = std::vector<std::vector<char>>(getHeight(), std::vector<char>(getWidth(), '.'));
}

Image* Paint::getIconImage() const {
    return _image.get();
}
std::string Paint::getTitle() const {
    return "Paint";
}

Pixel Paint::getPixel(const Coordinates& coords) const {
    Pixel pixel = _canvas[coords.y][coords.x];

    if (pixel.symbol != '.') {
        pixel.backgroundColor = BackgroundColor::White;
    } else {
        pixel.backgroundColor = BackgroundColor::Default;
    }

    return pixel;
}

void Paint::processMouseEvent(const MouseEvent& event) {
    if (!isVisible() || !isCliked(event.getCoords())) {
        return;
    }

    if (event.isClick()) {
        _isPainting = true;
    } else if (event.isUnclick()) {
        _isPainting = false;
    }

    if (!_isPainting) {
        return;
    }

    auto coords = globalCoordsToLocal(event.getCoords());
    int x = coords.x;
    int y = coords.y;
    int r = 1;
    for (int dx = -r; dx <= r; ++dx) {
        for (int dy = -r; dy <= r; ++dy) {
            if (x + dx >= 0 && x + dx < getWidth() &&
                y + dy >= 0 && y + dy < getHeight()) {
                _canvas[y + dy][x + dx] = '#';
            }
        }
    }
}

void Paint::processKey(char c) {
    if (!isVisible() || !getParent()->isFront()) {
        return;
    }

    if (c == 'q') {
        for (auto& row : _canvas) {
            for (auto& symbol : row) {
                symbol = '.';
            }
        }
    }
}

void Paint::resize(Size size) {
    _canvas = std::vector<std::vector<char>>(size.height, std::vector<char>(size.width, '.'));
    Window::resize(size);
}
