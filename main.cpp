#include <string>
#include <vector>
#include <iostream>
#include <Console/IO.hpp>
#include <Console/Graphics.hpp>


int main(int argc, char* argv[])
{
    IO io;

    if(io.validateArgs((uint32_t)argc, argv))
    {
        std::vector<std::string> columnsNames{ "Test1", "Test2", "Test3", "Test4", "Test5" };
        std::vector<std::vector<std::string>> rows(io.getInputArgValue(), { "left_columnX", "midd_columnX", "righ_columnX", "aaa", "bbb" });

        Graphics graphics(columnsNames, rows);
        graphics.display();
    }
    else
    {
        std::cout << "Invalid arguments!" << std::endl;
        std::cout << "Program accepts unsigned integer as number of required philosphers." << std::endl;
    }

    return 0;
}
