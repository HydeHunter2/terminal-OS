#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <map>
#include <sys/time.h>
#include <sys/select.h>

#include "operating_system.hpp"
#include "terminal.hpp"
#include "paint.hpp"

// todo: redraw only when something changed
// todo: Move time logic to some class
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

TimePoint now() {
    return std::chrono::high_resolution_clock::now();
}
int timeDifference(TimePoint firstPoint, TimePoint secondPoint) {
    return std::abs(std::chrono::duration_cast<std::chrono::milliseconds>(secondPoint - firstPoint).count());
}

int main() {
    system("bash -c \"clear && echo -n \"\e[3J\"\"");  // clear terminal

    system("stty raw");
    std::cout << "\e[?1002;1006;1015h";  // start observe mouse

    const int w = 200;
    const int h = 40;

    OperatingSystem OS({w, h});

    // temp hardcode
    OS.addWindow(Application<Terminal>(Rect(3, 3, 50, 20)).getWindow());
    OS.draw();

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

    while (true) {
        std::vector<char> keyData;
        std::vector<int> mouseData;
        int buffer = 0;

        select(1, &rfds, NULL, NULL, &timeout);
        if (FD_ISSET(0, &rfds)) {
            char c = getchar();

            if (c == 27) {
                getchar(); // [
                c = getchar();

                if ('0' <= c && c <= '9') {
                    while (c != 'M') {
                        if (c == ';') {
                            mouseData.push_back(buffer);
                            buffer = 0;
                        } else if ('0' <= c && c <= '9') {
                            buffer *= 10;
                            buffer += c - '0';
                        }

                        c = getchar();
                    }
                    mouseData.push_back(buffer);
                } else {
                    keyData.push_back('[');
                    keyData.push_back(c);
                }
            } else {
                keyData.push_back(c);
            }
        } else {
            FD_SET(0, &rfds);
        }

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
        } else if (keyData.size() >= 2 && keyData[0] == 91) {
            switch (keyData.at(1)) {
                case 'A': {
                    OS.processSpecialKey(SpecialKey::UpArrow);
                    break;
                }
                case 'B': {
                    OS.processSpecialKey(SpecialKey::DownArrow);
                    break;
                }
            }
        } else {
            for (char c : keyData) {
                if ((32 <= c && c <= 126) || c == 13 || c == 127) {
                    OS.processKey(c);
                }
            }
        }
        OS.tick();

        OS.draw();
    }

    std::cout << "\e[?1002;1006;1015l";  // stop observe mouse
    system("stty cooked");

    return 0;
}
