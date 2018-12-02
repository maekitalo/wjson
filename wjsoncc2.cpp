#include "person.h"
#include <cxxtools/arg.h>
#include <cxxtools/argout.h>
#include <cxxtools/json.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cxxtools::Arg<bool> beautify(argc, argv, 'b');
        cxxtools::ArgOut out(argc, argv);

        Person p;
        p.firstName = "John";
        p.lastName = "Smith";
        p.isAlive = true;
        p.age = 27;
        p.address.streetAddress = "21 2nd Street";
        p.address.city = "New York";
        p.address.state = "NY";
        p.address.postalCode = "10021-3100";
        p.phoneNumbers.resize(3);
        p.phoneNumbers[0].type = PhoneNumber::home;
        p.phoneNumbers[0].number = "212 555-1234";
        p.phoneNumbers[1].type = PhoneNumber::office;
        p.phoneNumbers[1].number = "646 555-4567";
        p.phoneNumbers[2].type = PhoneNumber::mobile;
        p.phoneNumbers[2].number = "123 456-7890";

        out << cxxtools::Json(p).beautify(beautify);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
