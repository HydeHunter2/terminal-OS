#pragma once

// refactor?
enum class Effect {
    Reset = 0,
    Bold = 1,
    Dim = 2,
    Italics = 3,
    Underlined = 4,
    Blink = 5,
    Reverse = 6,
    Hidden = 7
};
enum class ForgroundColor {
    Default = 39,
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    LightGray = 37,
    DarkGray = 90,
    LightRed = 91,
    LightGreen = 92,
    LightYellow = 93,
    LightBlue = 94,
    LightMagenta = 95,
    LightCyan = 96,
    White = 97
};
enum class BackgroundColor {
    Default = 49,
    Black = 40,
    Red = 41,
    Green = 42,
    Yellow = 43,
    Blue = 44,
    Magenta = 45,
    Cyan = 46,
    LightGray = 47,
    DarkGray = 100,
    LightRed = 101,
    LightGreen = 102,
    LightYellow = 103,
    LightBlue = 104,
    LightMagenta = 105,
    LightCyan = 106,
    White = 107
};

struct Pixel {
  public:
    Pixel(char c) : symbol(c) {}
    Pixel(char c, Effect e, ForgroundColor fc, BackgroundColor bg)
        : symbol(c), effect(e), forgroundColor(fc), backgroundColor(bg) {}

    char symbol = ' ';
    Effect effect = Effect::Reset;
    ForgroundColor forgroundColor = ForgroundColor::Default;
    BackgroundColor backgroundColor = BackgroundColor::Default;
};
