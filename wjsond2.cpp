#include "person.h"
#include <cxxtools/argin.h>
#include <cxxtools/json.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cxxtools::ArgIn in(argc, argv);

        Person p;
        std::cin >> cxxtools::Json(p);

        std::cout << p.firstName << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
