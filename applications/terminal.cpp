#include "terminal.hpp"

Terminal::Terminal(const Rect& rect) : Window(rect) {
    _image = std::unique_ptr<Image>(new Image(Size(9, 5),
        {'-', '-', '-', '-', '-', '-', '-', '-', '-',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '|', 'C', 'o', 'n', 's', 'o', 'l', 'e', '|',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '-', '-', '-', '-', '-', '-', '-', '-', '-'}));
}

Image* Terminal::getIconImage() {
    return _image.get();
}

char Terminal::getPixel(const Coordinates& coords) const {
    int flatCoord = coords.y * getWidth() + coords.x;
    int index = 0;

    for (const auto& str : _history) {
        int xPosition = 0;
        static auto newLineWithCheck = [this, &xPosition, &index, &flatCoord]() {
            int oldIndex = index;
            index += getWidth() - xPosition;
            xPosition = 0;
            return (oldIndex <= flatCoord && flatCoord < index);
        };

        for (char c : str) {
            if (c == '\n') {
                if (newLineWithCheck()) {
                    return ' ';
                }
            } else if (index == flatCoord) {
                return c;
            }
            
            if (c != '\n') {
                ++index;
                ++xPosition;

                if (xPosition == getWidth()) {
                    newLineWithCheck();
                }
            }
        }
        if (newLineWithCheck()) {
            return ' ';
        }
    }

    return ' ';
}

void Terminal::processKey(char c) {
    if (!isVisible() || !getParent()->isFront()) {
        return;
    }

    if (c == 13) {
        auto output = processCommand();
        if (!output.empty()) {
            _history.push_back(output);
        }

        _history.push_back(_name + "> ");
    } else if (c == 127) {
        auto& command = _history.back();
        if (command.size() > _name.size() + 2) {
            command.pop_back();
        }
    } else {
        _history.back() += c;
    }
}

std::string Terminal::processCommand() {
    // todo: delete all space in begin and end
    auto command = _history.back();
    command.erase(0, _name.size() + 2);

    if (command == "help") {
        return " - help\n - exit";
    } else if (command == "exit") {
        getParent()->kill();
    } else if (command == "") {
        return "";
    }

    return "  Command not found: " + command + ". Use \"help\"";
}
