#include <Console/WindowContent.hpp>


WindowContent::WindowContent(uint32_t xPos, uint32_t yPos, uint32_t width, uint32_t height, const std::vector<std::string>& columnsNames, WINDOW* window)
    : _menuItems(nullptr)
    , _columnsNames(columnsNames)
    , _xPos(xPos)
    , _yPos(yPos)
    , _width(width)
    , _height(height)
    , _rowsCount(0)
    , _columnsCount(columnsNames.size())
    , _columnWidth(calculateColumnWidth())
    , _baseMenuItemId(2)
{
    initWindowContent(window);
}

WindowContent::~WindowContent()
{
    unpost_menu(_menu);
    free_menu(_menu);

    for(uint32_t rowId = 0; rowId < _rowsCount; ++rowId)
    {
        free_item(_menuItems[rowId]);
    }
}

void WindowContent::initWindowContent(WINDOW* window)
{
    initColumnsTitles();
    _menu = new_menu(_menuItems);
    set_menu_win(_menu, window);
    set_menu_sub(_menu, derwin(window, _height, _width, _yPos, _xPos));
    set_menu_format(_menu, _height, _width);
    set_menu_mark(_menu, "");

    post_menu(_menu);
}

void WindowContent::initColumnsTitles()
{
    addNewRow(_columnsNames);
    addNewRow(std::vector<std::string>(_columnsCount, std::string(_columnWidth, '-')));
}

void WindowContent::extendMenuByOneRow()
{
    ITEM** newMenuItems = new ITEM*[++_rowsCount];

    for(uint32_t rowId = 0; rowId < _rowsCount - 1; ++rowId)
    {
        newMenuItems[rowId] = _menuItems[rowId];
    }

    if(_menuItems)
    {
        delete _menuItems;
    }

    _menuItems = newMenuItems;
}

std::string WindowContent::createRow(const std::vector<std::string>& columnsValues)
{
    std::string result;
    uint32_t bordersCount = 0;

    for(const auto& columnValue : columnsValues)
    {
        uint32_t padding = _columnWidth - columnValue.size();

        result = result + columnValue + std::string(padding, ' ');

        if(++bordersCount != columnsValues.size())
        {
            result = result + "|";
        }
    }

    return result;
}

bool WindowContent::addNewRow(const std::vector<std::string>& columnsValues)
{
    if(columnsValues.size() != _columnsNames.size())
    {
        return false;
    }

    extendMenuByOneRow();
    std::string row = createRow(columnsValues);
    _menuItems[_rowsCount - 1] = new_item(row.c_str(), row.c_str());

    return true;
}

uint32_t WindowContent::calculateColumnWidth()
{
    return (_width - (_columnsCount - 1)) / _columnsCount;
}
