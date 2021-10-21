#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "operating_system.hpp"
#include "bordered_window.hpp"

int main() {
    const int w = 200;
    const int h = 30;

    OperatingSystem OS({w, h});

    // temp hardcode
    BorderedWindow* window1 = new BorderedWindow(Rect(15, 10, 50, 5));
    BorderedWindow* window2 = new BorderedWindow(Rect(5, 5, 50, 15));
    OS.addWindow(window1);
    OS.addWindow(window2);

    system("bash -c \"clear && echo -n \"\e[3J\"\"");  // clear terminal
    OS.draw();

    std::string input;
    while (true) {
        system("stty raw");
        system("echo \"\e[?1002;1006;1015h\"");  // start observe mouse

        std::getline(std::cin, input, char(27));
        if (input.length() == 0) {
            continue;
        }

        system("echo \"\e[?1002;1006;1015l\"");  // stop observe mouse
        system("stty cooked");

        std::vector<int> mouseData;

        int data = 0;
        for (char c : input) {
            if (c == ';') {
                mouseData.push_back(data);
                data = 0;
            } else if ('0' <= c && c <= '9') {
                data *= 10;
                data += c - '0';
            }
        }
        mouseData.push_back(data);

        int mouseX = mouseData[1] - 1;
        int mouseY = mouseData[2] - 1;

        if (mouseX + mouseY == 0) {
            system("bash -c \"clear && echo -n \"\e[3J\"\"");
            break;
        }

        system("bash -c \"clear && echo -n \"\e[3J\"\"");
        if (mouseX >= 0 && mouseX < w &&
            mouseY >= 0 || mouseY < h) {
            auto type = (mouseData[0] == 32 ? MouseEvent::Type::Click : MouseEvent::Type::Drag);
            OS.processMouseEvent({{mouseX, mouseY}, type});
        }
        OS.tick();
        OS.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return 0;
}
