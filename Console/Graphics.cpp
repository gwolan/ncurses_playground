#include <Console/Graphics.hpp>


Graphics::Graphics(std::vector<std::string> columnsNames)
    : _columnsNames(columnsNames)
    , _topPadding(4)
    , _bottomPadding(4)
    , _horizontalPadding(4)
{
    init();
}

Graphics::~Graphics()
{
    endwin();
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
                //refresh();
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

uint32_t Graphics::calculateWindowWidth()
{
    return COLS - 2*_horizontalPadding;
}

uint32_t Graphics::calculateWindowHeight()
{
    return LINES - _topPadding - _bottomPadding;
}

void Graphics::init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, true);

    _mainWindowPtr = std::make_unique<Window>(_horizontalPadding, _topPadding, calculateWindowWidth(), calculateWindowHeight(), _columnsNames);
    refresh();
}
