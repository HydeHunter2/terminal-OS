#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "file_system.hpp"
#include "bordered_window.hpp"
#include "application.hpp"
#include "paint.hpp"

// todo: scroll
class Terminal : public Window {
  public:
    Terminal(const Rect& rect);

    Image* getIconImage() const;
    std::string getTitle() const;

    Pixel getPixel(const Coordinates& coords) const override;
    void processSpecialKey(SpecialKey specialKey) override;
    void processKey(char c) override;
    void resize(Size size) override;

  private:
    class Command {
      public:
        Command(const std::string& prefix, const std::vector<std::string>& tokens, const std::string& output);

        std::string getPrefix() const;
        std::string getOutput() const;
        std::vector<std::string> getTokens() const;
        std::string getTokensString() const;

      private:
        std::string _prefix;
        std::vector<std::string> _tokens;
        std::string _output{ "" };
    };

    std::unique_ptr<Image> _image{ nullptr };
    std::string _name{ "hyde" };
    std::vector<Command> _history{};
    int _selectPrevCommandIndex;
    std::vector<std::vector<Pixel>> _canvas;
    std::string _input{ "" };

    void updateCanvas();
    std::string getPrefix() const;

    std::string processCommand(const std::vector<std::string>& tokens);
    std::string help(const std::vector<std::string>& tokens);
    std::string ls(const std::vector<std::string>& tokens);
    std::string cat(const std::vector<std::string>& tokens);
    std::string terminal(const std::vector<std::string>& tokens);
    std::string paint(const std::vector<std::string>& tokens);
};
