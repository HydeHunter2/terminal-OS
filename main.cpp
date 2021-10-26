#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>

#include "operating_system.hpp"
#include "bordered_window.hpp"

// todo: Move time logic to some class
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

TimePoint now() {
    return std::chrono::high_resolution_clock::now();
}
int timeDifference(TimePoint firstPoint, TimePoint secondPoint) {
    return std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(secondPoint - firstPoint).count());
}

int main() {
    const int w = 200;
    const int h = 30;

    OperatingSystem OS({w, h});

    // temp hardcode
    BorderedWindow* window1 = new BorderedWindow(Rect(15, 10, 50, 5), OS.getTaskbar());
    BorderedWindow* window2 = new BorderedWindow(Rect(5, 5, 50, 15), OS.getTaskbar());
    OS.addWindow(window1);
    OS.addWindow(window2);

    system("bash -c \"clear && echo -n \"\e[3J\"\"");  // clear terminal
    OS.draw();

    std::string input;
    while (true) {
        system("stty raw");
        std::cout << "\e[?1002;1006;1015h";  // start observe mouse

        std::vector<int> mouseData;
        int data = 0;

        char c;
        auto startClickTime = now();
        while ((c = getchar()) != 27) {  // todo: process keyboard keys
            if (timeDifference(startClickTime, now()) > 100) {
                break;
            }

            if (c == ';') {
                mouseData.push_back(data);
                data = 0;
            } else if ('0' <= c && c <= '9') {
                data *= 10;
                data += c - '0';
            }
        }
        mouseData.push_back(data);

        std::cout << "\e[?1002;1006;1015l";  // start observe mouse
        system("stty cooked");

        if (mouseData.size() != 3) {
            continue;
        }

        int mouseX = mouseData[1] - 1;
        int mouseY = mouseData[2] - 1;

        if (mouseX + mouseY == 0) {
            system("bash -c \"clear && echo -n \"\e[3J\"\"");
            break;
        }

        system("bash -c \"clear && echo -n \"\e[3J\"\"");
        if (mouseX >= 0 && mouseX < w &&
            mouseY >= 0 || mouseY < h) {
            static std::map<int, MouseEvent::Type> keyToType = {
                {32, MouseEvent::Type::Click},
                {35, MouseEvent::Type::Unclick},
                {64, MouseEvent::Type::Drag},
            };
            OS.processMouseEvent({{mouseX, mouseY}, keyToType[mouseData[0]]});
        }
        OS.tick();
        OS.draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    return 0;
}
