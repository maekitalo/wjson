#include <cxxtools/serializationinfo.h>
#include <cxxtools/json.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cxxtools::SerializationInfo si;
        si.addMember("firstName") <<= "John";
        si.addMember("lastName") <<= "Smith";
        si.addMember("age") <<= 27;

        std::cout << cxxtools::Json(si).beautify(true);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
