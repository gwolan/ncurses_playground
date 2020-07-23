#pragma once

#include <menu.h>
#include <ncurses.h>
#include <cstdint>
#include <memory>
#include <vector>

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
    void initColumnsTitles();
    uint32_t calculateColumnWidth();
    bool addNewRow(const std::vector<std::string>& columnsValues);
    std::string createRow(const std::vector<std::string>& columnsValues);
    void extendMenuByOneRow();

    WINDOW* _window;
    MENU* _menu;
    ITEM** _menuItems;

    std::vector<std::string> _columnsNames;
    const uint32_t _topPadding;
    const uint32_t _bottomPadding;
    const uint32_t _horizontalPadding;
    uint32_t _rowsCount;
    uint32_t _columnsCount;
    uint32_t _columnWidth;
    const uint32_t _baseMenuItemId;
};
