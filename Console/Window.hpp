#pragma once

#include <ncurses.h>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <Console/WindowContent.hpp>


class Window
{
    public:
    Window(uint32_t xPos, uint32_t yPos, uint32_t width, uint32_t height, const std::vector<std::string>& columnsNames);
    ~Window();

    void refresh();
    void initWindow();


    private:
    WINDOW* _window;
    std::unique_ptr<WindowContent> _windowContentPtr;
    const std::vector<std::string>& _columnsNames;
    uint32_t _xPos;
    uint32_t _yPos;
    uint32_t _width;
    uint32_t _height;
};
