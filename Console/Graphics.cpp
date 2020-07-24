#include <Console/Graphics.hpp>


Graphics::Graphics(const std::vector<std::string>& columnsNames, const std::vector<std::vector<std::string>>& rowsValues)
    : _window(nullptr)
    , _menu(nullptr)
    , _menuItems(nullptr)
    , _columnsNames(columnsNames)
    , _rowsValues(rowsValues)
    , _rows()
    , _nullRow("\0")
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPadding(4)
    , _columnsCount(columnsNames.size())
    , _columnWidth()
    , _rowsCount()
    , _baseMenuItemId(2)
    , log("Graphics::")
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
        else
        {
            padding = calculateWindowWidth() - 2 - result.size();
            result = result + std::string(padding, ' ');
        }
    }

    return result;
}

void Graphics::createRows()
{
    std::string menuTitle = createRow(_columnsNames);
    std::string lineSeparator = createRow(std::vector<std::string>(_columnsCount, std::string(_columnWidth, '-')));
    _rows.push_back(menuTitle);
    _rows.push_back(lineSeparator);

    for(const auto& row : _rowsValues)
    {
        std::string menuItem = createRow(row);

        _rows.push_back(menuItem);
    }

    _rows.push_back(_nullRow);
    _rowsCount = _rows.size();
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

void Graphics::initMenuItems()
{
    _menuItems = new ITEM*[_rows.size()];
    for(uint32_t rowId = 0; rowId < _rows.size() - 1; ++rowId)
    {
        _menuItems[rowId] = new_item(_rows[rowId].c_str(), _rows[rowId].c_str());
    }

    _menuItems[_rows.size() - 1] = new_item(_rows.rbegin()->c_str(), _rows.rbegin()->c_str());
}

void Graphics::init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    _columnWidth = calculateColumnWidth();
    uint32_t windowHeight = calculateWindowHeight();
    uint32_t windowWidth = calculateWindowWidth();
    uint32_t menuHeight = calculateWindowHeight() - 2;
    uint32_t menuWidth = calculateWindowWidth() - 2;

    createRows();
    initMenuItems();
    _menu = new_menu(_menuItems);
    _window = newwin(windowHeight, windowWidth, _topPadding, _horizontalPadding);
    keypad(_window, true);

    set_menu_win(_menu, _window);
    set_menu_sub(_menu, derwin(_window, menuHeight, menuWidth, 1, 1));
    set_menu_format(_menu, menuHeight, 1);
    set_menu_mark(_menu, "");

    box(_window, 0, 0);
    refresh();

    post_menu(_menu);
    wrefresh(_window);
    refresh();
}

void Graphics::refreshMenu()
{
    ITEM* currentItem = _menu->curitem;

    unpost_menu(_menu);
    set_menu_items(_menu, _menuItems);
    set_current_item(_menu, currentItem);
    post_menu(_menu);
    wrefresh(_window);
}

void Graphics::display()
{
    uint32_t idx = _baseMenuItemId;
    uint32_t appValue = 0;
    uint32_t selection = 0;
    std::string newRow;

    int32_t option;
    bool quit = false;

    while(!quit && (option = getch()))
    {
        switch(option)
        {
            case KEY_DOWN:
                menu_driver(_menu, REQ_DOWN_ITEM);
                selection++;
                break;
            case KEY_UP:
                menu_driver(_menu, REQ_UP_ITEM);
                selection--;
                break;
            case KEY_NPAGE:
                menu_driver(_menu, REQ_SCR_DPAGE);
                break;
            case KEY_PPAGE:
                menu_driver(_menu, REQ_SCR_UPAGE);
                break;
            case 'r':
                newRow = createRow({ "left_column" + std::to_string(appValue), "midd_column" + std::to_string(appValue), "righ_column" + std::to_string(appValue) });
                _rows[idx] = newRow;
                _menuItems[idx] = new_item(_rows[idx].c_str(), _rows[idx].c_str());
                idx++;
                refreshMenu();
                break;
            case 10:
                appValue++;
                newRow = createRow({ "left_column" + std::to_string(appValue), "midd_column" + std::to_string(appValue), "righ_column" + std::to_string(appValue) });
                _rows[selection] = newRow;
                _menuItems[selection] = new_item(_rows[selection].c_str(), _rows[selection].c_str());
                refreshMenu();
                break;
            case 'q':
                quit = true;
                break;
            default:
                break;
        }

        if(idx == _rowsCount - 1)
        {
            idx = _baseMenuItemId;
            appValue++;
        }

        wrefresh(_window);
    }
}
