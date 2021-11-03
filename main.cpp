#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>

#include "operating_system.hpp"
#include "terminal.hpp"
#include "paint.hpp"

// todo: colorful cout (!)
// todo: process system keys (control/option/shift)

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
    const int h = 40;

    OperatingSystem OS({w, h});

    // temp hardcode
    OS.addWindow(Application<Terminal>(Rect(3, 3, 50, 20)).getWindow());
    OS.addWindow(Application<Paint>(Rect(7, 7, 50, 20)).getWindow());

    system("bash -c \"clear && echo -n \"\e[3J\"\"");  // clear terminal
    OS.draw();

    while (true) {
        system("stty raw");
        std::cout << "\e[?1002;1006;1015h";  // start observe mouse

        std::vector<int> mouseData;
        int data = 0;

        char c;
        auto startClickTime = now();
        while (true) {
            c = getchar();

            if (timeDifference(startClickTime, now()) > 20) {
                break;
            }

            if (c == ';') {
                mouseData.push_back(data);
                data = 0;
            } else if ('0' <= c && c <= '9') {
                data *= 10;
                data += c - '0';
            } else if (c == 'M' && mouseData.size() == 2) {
                break;
            }
        }
        mouseData.push_back(data);

        std::cout << "\e[?1002;1006;1015l";  // start observe mouse
        system("stty cooked");

        if (mouseData.size() == 3) {
            int mouseX = mouseData[1] - 1;
            int mouseY = mouseData[2] - 1;

            if (mouseX == 0 && mouseY == 0) {
                system("bash -c \"clear && echo -n \"\e[3J\"\"");
                break;
            }

            if (mouseX >= 0 && mouseX < w &&
                mouseY >= 0 || mouseY < h) {
                static std::map<int, MouseEvent::Type> keyToType = {
                    {32, MouseEvent::Type::Click},
                    {35, MouseEvent::Type::Unclick},
                    {64, MouseEvent::Type::Drag},
                };
                OS.processMouseEvent({{mouseX, mouseY}, keyToType[mouseData[0]]});
            }
        } else {
            if ((32 <= c && c <= 126) || c == 13 || c == 127) {
                OS.processKey(c);
            }
        }
        OS.tick();

        system("bash -c \"clear && echo -n \"\e[3J\"\"");
        OS.draw();
    }

    return 0;
}
