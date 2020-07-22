#include <Console/Window.hpp>


Window::Window(uint32_t xPos, uint32_t yPos, uint32_t width, uint32_t height, const std::vector<std::string>& columnsNames)
    : _windowContentPtr(nullptr)
    , _columnsNames(columnsNames)
    , _xPos(xPos)
    , _yPos(yPos)
    , _width(width)
    , _height(height)
{
    initWindow();
}

Window::~Window()
{
    wborder(_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    refresh();

    delwin(_window);
}

void Window::initWindow()
{
    _window = newwin(_height, _width, _yPos, _xPos);
    box(_window, 0, 0);

    _windowContentPtr = std::make_unique<WindowContent>(_xPos + 1, _yPos + 1, _width - 1, _height - 1, _columnsNames, _window);
    wrefresh(_window);
}

void Window::refresh()
{
    wrefresh(_window);
}
