#pragma once

#include <ncurses.h>
#include <cstdint>
#include <memory>
#include <vector>
#include <Console/Window.hpp>

class Graphics
{

    public:
    Graphics(std::vector<std::string> columnsNames);
    ~Graphics();

    void display();


    private:
    void init();
    uint32_t calculateWindowWidth();
    uint32_t calculateWindowHeight();

    std::unique_ptr<Window> _mainWindowPtr;
    std::vector<std::string> _columnsNames;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPadding;
};
