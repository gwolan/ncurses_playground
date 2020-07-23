#include <Console/Graphics.hpp>


Graphics::Graphics(std::vector<std::string> columnsNames)
    : _window(nullptr)
    , _columnsNames(columnsNames)
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPadding(4)
    , _rowsCount(0)
    , _columnsCount(columnsNames.size())
    , _columnWidth(calculateColumnWidth())
    , _baseMenuItemId(2)
{
    init();
}

Graphics::~Graphics()
{
    wrefresh(_window);
    unpost_menu(_menu);
    free_menu(_menu);

    for(uint32_t rowId = 0; rowId < _rowsCount; ++rowId)
    {
        free_item(_menuItems[rowId]);
    }

    wborder(_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    refresh();
    delwin(_window);

    endwin();
}

uint32_t Graphics::calculateWindowWidth()
{
    return COLS - 2*_horizontalPadding;
}

uint32_t Graphics::calculateWindowHeight()
{
    return LINES - _topPadding - _bottomPadding;
}

uint32_t Graphics::calculateColumnWidth()
{
    return ((calculateWindowWidth() - 1) - (_columnsCount - 1)) / _columnsCount;
}

void Graphics::initColumnsTitles()
{
    addNewRow(_columnsNames);
    addNewRow(std::vector<std::string>(_columnsCount, std::string(_columnWidth, '-')));

    std::string n("\0");
    extendMenuByOneRow();
    _menuItems[_rowsCount - 1] = new_item(n.c_str(), n.c_str());
}

void Graphics::init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    uint32_t windowHeight = calculateWindowHeight();
    uint32_t windowWidth = calculateWindowWidth();
    uint32_t menuHeight = calculateWindowHeight() - 1;
    uint32_t menuWidth = calculateWindowWidth() - 1;

    initColumnsTitles();
    _menu = new_menu(_menuItems);
    _window = newwin(windowHeight, windowWidth, _topPadding, _horizontalPadding);
    keypad(_window, true);

    set_menu_win(_menu, _window);
    set_menu_sub(_menu, derwin(_window, menuHeight, menuWidth, 1, 1));
    set_menu_format(_menu, menuHeight, menuWidth);
    set_menu_mark(_menu, "");

    box(_window, 0, 0);
    refresh();

    post_menu(_menu);
    wrefresh(_window);
    refresh();
}

void Graphics::display()
{
    int32_t option;
    bool finished = false;

    while(!finished && (option = getch()))
    {
        switch(option)
        {
            case 'r':
                wrefresh(_window);
                break;
            case 'q':
                finished = true;
                break;
            default:
                break;
        }

        refresh();
    }
}

void Graphics::extendMenuByOneRow()
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

std::string Graphics::createRow(const std::vector<std::string>& columnsValues)
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

bool Graphics::addNewRow(const std::vector<std::string>& columnsValues)
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
