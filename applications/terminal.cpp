#include "terminal.hpp"

Terminal::Terminal(const Rect& rect) : Window(rect) {
    _image = std::unique_ptr<Image>(new Image(Size(9, 5),
        {'-', '-', '-', '-', '-', '-', '-', '-', '-',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '|', 'C', 'o', 'n', 's', 'o', 'l', 'e', '|',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '-', '-', '-', '-', '-', '-', '-', '-', '-'}));
}

Image* Terminal::getIconImage() const {
    return _image.get();
}
std::string Terminal::getTitle() const {
    return "Terminal";
}

Pixel Terminal::getPixel(const Coordinates& coords) const {
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

std::string removeLeadingAndTrailingSpaces(const std::string& str) {
    auto start = str.find_first_not_of(" ");
    auto end = str.find_last_not_of(" ") + 1;
    if (start == std::string::npos) {
        return "";
    }

    return str.substr(start, end - start);
}
std::vector<std::string> slpit(const std::string& str, char delimitor, bool deleteEmpty = true) {
    std::vector<std::string> out;

    std::string s;
    std::stringstream stringstream(str);
    while (std::getline(stringstream, s, delimitor)) {
        if (!deleteEmpty || !s.empty()) {
            out.push_back(s); 
        }
    }

    return out;
} 

std::string Terminal::processCommand() {
    auto command = _history.back();
    command.erase(0, _name.size() + 2);
    command = removeLeadingAndTrailingSpaces(command);
    if (command.empty()) {
        return "";
    }

    auto tokens = slpit(command, ' ');
    if (tokens[0] == "help") {
        return help(tokens);
    } else if (tokens[0] == "terminal") {
        return terminal(tokens);
    } else if (tokens[0] == "paint") {
        return paint(tokens);
    } else if (tokens[0] == "exit") {
        getParent()->kill();
    }

    return "  Command not found: " + command + ". Use \"help\"";
}

std::string invalidArgument(const std::string& argument, const std::string& command) {
    return "  Invalid argument: " + argument + ". See \"man " + command + "\" or \"" + command + " -h\"";
}
std::string Terminal::help(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return invalidArgument(tokens[1], "help");
    }
    return " - help\n - terminal\n - paint\n - exit";
}
std::string Terminal::terminal(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return invalidArgument(tokens[1], "terminal");
    }

    // fix rect
    getOS()->addWindow(Application<Terminal>(Rect(7, 7, 50, 20)).getWindow());
    return "  Start Terminal...";
}
std::string Terminal::paint(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return invalidArgument(tokens[1], "paint");
    }

    // fix rect
    getOS()->addWindow(Application<Paint>(Rect(7, 7, 50, 20)).getWindow());
    return "  Start Paint...";
}
