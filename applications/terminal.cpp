#include "terminal.hpp"

Terminal::Terminal(const Rect& rect) : Window(rect) {
    _image = std::unique_ptr<Image>(new Image(Size(9, 5),
        {'-', '-', '-', '-', '-', '-', '-', '-', '-',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '|', 'C', 'o', 'n', 's', 'o', 'l', 'e', '|',
         '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|',
         '-', '-', '-', '-', '-', '-', '-', '-', '-'}));

    _canvas = std::vector<std::vector<Pixel>>(getHeight(), std::vector<Pixel>(getWidth(), {' '}));
    updateCanvas();
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

    return _canvas[coords.y][coords.x];
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

void Terminal::processKey(char c) {
    if (!isVisible() || !getParent()->isFront()) {
        return;
    }

    if (c == 13) {
        auto tokens = slpit(removeLeadingAndTrailingSpaces(_input), ' ');
        if (!tokens.empty()) {
            auto output = processCommand(tokens);
            _history.push_back({getPrefix(), tokens, output});
            _selectPrevCommandIndex = _history.size();
        }

        _input = "";
    } else if (c == 127) {
        if (!_input.empty()) {
            _input.pop_back();
        }
    } else {
        _input += c;
    }
    updateCanvas();

    Window::processKey(c);
}
void Terminal::processSpecialKey(SpecialKey specialKey) {
    if (!isVisible() || !getParent()->isFront()) {
        return;
    }

    switch (specialKey) {
        case SpecialKey::UpArrow: {
            if (_selectPrevCommandIndex != 0) {
                --_selectPrevCommandIndex;
            }
            break;
        }
        case SpecialKey::DownArrow: {
            if (_selectPrevCommandIndex < _history.size()) {
                ++_selectPrevCommandIndex;
            }
            break;
        }
    }
    if (_selectPrevCommandIndex == _history.size()) {
        _input = "";
    } else {
        _input = _history[_selectPrevCommandIndex].getTokensString();
    }
    updateCanvas();

    Window::processSpecialKey(specialKey);
}

void Terminal::updateCanvas() {
    for (auto& line : _canvas) {
        for (auto& pixel : line) {
            pixel = ' ';
        }
    }

    int x = 0;
    int y = 0;

    auto nextLine = [&x, &y]() {
        ++y;
        x = 0;
    };
    auto nextSymbol = [this, &nextLine, &x, &y]() {
        ++x;
        if (x == getWidth()) {
            nextLine();
        }
    };
    auto printString = [this, &nextSymbol, &nextLine, &x, &y]
        (const std::string& str,
         Effect effect = Effect::Reset,
         ForgroundColor fg = ForgroundColor::Default,
         BackgroundColor bg = BackgroundColor::Default) {
        for (char c : str) {
            if (c == '\n') {
                nextLine();
            } else {
                _canvas[y][x] = {c, effect, fg, bg};
                nextSymbol();   
            }
        }
    };
    auto printPrefix = [&printString](const std::string& prefix) {
        printString(prefix, Effect::Bold, ForgroundColor::LightGreen);
    };

    for (const auto& command : _history) {
        printPrefix(command.getPrefix());
        for (const auto& token : command.getTokens()) {
            printString(token);
            nextSymbol();
        }
        nextLine();
        printString(command.getOutput());

        ++y;
        x = 0;
    }

    printPrefix(getPrefix());
    printString(_input);

    _canvas[y][x] = {' ', Effect::Blink, ForgroundColor::Default, BackgroundColor::LightGray};
}

std::string Terminal::getPrefix() const {
    return _name + " $ ";
}

Terminal::Command::Command(const std::string& prefix, const std::vector<std::string>& tokens, const std::string& output)
    : _prefix(prefix), _tokens(tokens), _output(output) {}

std::string Terminal::Command::getPrefix() const {
    return _prefix;
}
std::string Terminal::Command::getOutput() const {
    return _output;
}
std::vector<std::string> Terminal::Command::getTokens() const {
    return _tokens;
}
std::string Terminal::Command::getTokensString() const {
    std::string out = "";
    for (int i = 0; i < _tokens.size(); ++i) {
        out += _tokens[i];
        if (i != _tokens.size() - 1) {
            out += '\n';
        }
    }
    return out;
}

std::string Terminal::processCommand(const std::vector<std::string>& tokens) {
    if (tokens[0] == "help") {
        return help(tokens);
    } else if (tokens[0] == "ls") {
        return ls(tokens);
    } else if (tokens[0] == "cat") {
        return cat(tokens);
    } else if (tokens[0] == "terminal") {
        return terminal(tokens);
    } else if (tokens[0] == "paint") {
        return paint(tokens);
    } else if (tokens[0] == "exit") {
        getParent()->kill();
    }

    return "  Command not found: " + tokens[0] + ". Use \"help\"";
}

void Terminal::resize(Size size) {
    _canvas = std::vector<std::vector<Pixel>>(getHeight() + 1, std::vector<Pixel>(getWidth() + 1, {' '}));
    updateCanvas();
    Window::resize(size);
}

std::string invalidArgument(const std::string& argument, const std::string& command) {
    return "  Invalid argument: " + argument + ". See \"man " + command + "\" or \"" + command + " -h\"";
}
std::string missingArguments(const std::string& command) {
    return "  Missing argument. See \"man " + command + "\" or \"" + command + " -h\"";
}

std::string Terminal::help(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return invalidArgument(tokens[1], "help");
    }
    return " - help\n - ls\n - terminal\n - paint\n - exit";
}
std::string Terminal::ls(const std::vector<std::string>& tokens) {
    if (tokens.size() > 1) {
        return invalidArgument(tokens[1], "help");
    }
    std::string out;

    auto files = FileSystem::ls("");
    for (int i = 0; i < files.size(); ++i) {
        out += files.at(i);

        if (i != files.size() - 1) {
            out += '\n';
        }
    }

    return out;
}
std::string Terminal::cat(const std::vector<std::string>& tokens) {
    if (tokens.size() == 1) {
        return missingArguments("cat");
    } else if (tokens.size() > 2) {
        return invalidArgument(tokens[2], "help");
    }

    auto file = FileSystem::File(tokens[1]);
    return FileSystem::read(file);
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
