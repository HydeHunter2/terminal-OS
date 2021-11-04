#pragma once

#include <iostream>
#include <string>

#include "pixel.hpp"

class ColorfulPrint {
  public:
    friend ColorfulPrint& operator<<(ColorfulPrint& os, const std::string& str) {
        std::cout << str;
        return os;
    }
    friend ColorfulPrint& operator<<(ColorfulPrint& os, const Pixel& pixel) {
        os._effect = pixel.effect;
        os._forgroundColor = pixel.forgroundColor;
        os._backgroundColor = pixel.backgroundColor;

        if (os._effect == Effect::Reset &&
            os._forgroundColor == ForgroundColor::Default &&
            os._backgroundColor == BackgroundColor::Default) {
            std::cout << pixel.symbol;
            return os;
        }

        std::cout << "\e["
                  << static_cast<int>(pixel.effect) << ";"
                  << static_cast<int>(pixel.forgroundColor) << ";"
                  << static_cast<int>(pixel.backgroundColor) << "m"
                  << pixel.symbol << "\e[0;39;49m";
        return os;
    }

  private:
    Effect _effect = Effect::Reset;
    ForgroundColor _forgroundColor = ForgroundColor::Default;
    BackgroundColor _backgroundColor = BackgroundColor::Default;
};
