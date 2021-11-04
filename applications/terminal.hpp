#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "bordered_window.hpp"
#include "application.hpp"
#include "paint.hpp"

// todo: file system (!!!)
// todo: add blinking symbol at carriage position (in tick method process blinking)
// todo: scroll
class Terminal : public Window {
  public:
    Terminal(const Rect& rect);

    Image* getIconImage() const;
    std::string getTitle() const;

    Pixel getPixel(const Coordinates& coords) const override;
    void processKey(char c) override;

  private:
    std::unique_ptr<Image> _image{ nullptr };
    const std::string _name{ "hyde" };
    std::vector<std::string> _history{ _name + "> " };

    std::string processCommand();
    std::string help(const std::vector<std::string>& tokens);
    std::string terminal(const std::vector<std::string>& tokens);
    std::string paint(const std::vector<std::string>& tokens);
};
