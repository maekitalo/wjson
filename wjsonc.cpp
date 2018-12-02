#include <cxxtools/serializationinfo.h>
#include <cxxtools/json.h>
#include <cxxtools/arg.h>
#include <cxxtools/argout.h>

#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cxxtools::Arg<bool> beautify(argc, argv, 'b');
        cxxtools::ArgOut out(argc, argv);

        cxxtools::SerializationInfo si;
        si.addMember("firstName") <<= "John";
        si.addMember("lastName") <<= "Smith";
        si.addMember("isAlive") <<= true;
        si.addMember("age") <<= 27;
        auto& ai = si.addMember("address");
        ai.addMember("streetAddress") <<= "21 2nd Street";
        ai.addMember("city") <<= "New York";
        ai.addMember("state") <<= "NY";
        ai.addMember("postalCode") <<= "10021-3100";
        auto& pi = si.addMember("phoneNumbers");
        pi.setCategory(cxxtools::SerializationInfo::Array);

        auto& pi0 = pi.addMember();
        pi0.addMember("type") <<= "home";
        pi0.addMember("number") <<= "212 555-1234";

        auto& pi1 = pi.addMember();
        pi1.addMember("type") <<= "office";
        pi1.addMember("number") <<= "646 555-4567";

        auto& pi2 = pi.addMember();
        pi2.addMember("type") <<= "mobile";
        pi2.addMember("number") <<= "123 456-7890";

        si.addMember("children").setCategory(cxxtools::SerializationInfo::Array);
        si.addMember("spouse");

        out << cxxtools::Json(si).beautify(beautify);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
