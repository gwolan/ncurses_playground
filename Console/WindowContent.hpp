#pragma once

#include <menu.h>
#include <cstdint>
#include <string>
#include <vector>


class WindowContent
{
    public:
    WindowContent(uint32_t xPos, uint32_t yPos, uint32_t width, uint32_t height, const std::vector<std::string>& columnsNames, WINDOW* window);
    ~WindowContent();


    private:
    void initWindowContent(WINDOW* window);
    void initColumnsTitles();
    uint32_t calculateColumnWidth();
    bool addNewRow(const std::vector<std::string>& columnsValues);
    std::string createRow(const std::vector<std::string>& columnsValues);
    void extendMenuByOneRow();

    MENU* _menu;
    ITEM** _menuItems;

    const std::vector<std::string>& _columnsNames;
    uint32_t _xPos;
    uint32_t _yPos;
    uint32_t _width;
    uint32_t _height;
    uint32_t _rowsCount;
    uint32_t _columnsCount;
    uint32_t _columnWidth;
    const uint32_t _baseMenuItemId;
};
