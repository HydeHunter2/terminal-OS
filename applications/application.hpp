#pragma once

#include "bordered_window.hpp"

// wrapper over Terminal, Paint and so on
template<class App>
class Application {
  public:
    template<class... Args>
    Application(Args... args) : _app(new App(args...)), _borderedWindow(new BorderedWindow(_app)) {
        _borderedWindow->setIcon(new Icon(_borderedWindow, _app->getIconImage()));
    }

    Window* getWindow() const {
        return _borderedWindow;
    }

  private:
    App* _app;
    BorderedWindow* _borderedWindow;
};
