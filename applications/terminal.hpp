#pragma once

#include <vector>
#include <string>

#include "bordered_window.hpp"

// todo: add blinking symbol at carriage position (in tick method process blinking)
// todo: scroll
class Terminal : public Window {
  public:
    Terminal(const Rect& rect);

    Image* getIconImage();

    char getPixel(const Coordinates& coords) const override;
    void processKey(char c) override;

  private:
    std::unique_ptr<Image> _image{ nullptr };
    const std::string _name{ "hyde" };
    std::vector<std::string> _history{ _name + "> " };

    std::string processCommand();
};
