#include <cxxtools/serializationinfo.h>
#include <cxxtools/json.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cxxtools::SerializationInfo si;
        std::cin >> cxxtools::Json(si);

        std::string firstName;
        std::string lastName;
        unsigned age;

        si.getMember("firstName") >>= firstName;
        si.getMember("lastName") >>= lastName;
        si.getMember("age") >>= age;

        std::cout << "first name: " << firstName << "\n"
                     "last name: " << lastName << "\n"
                     "age: " << age << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
