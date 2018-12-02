#include <cxxtools/arg.h>
#include <cxxtools/argout.h>
#include <cxxtools/json.h>
#include <cxxtools/serializationinfo.h>
#include <boost/optional.hpp>
#include <iostream>

// used structures

struct Address
{
    std::string streetAddress;
    std::string city;
    std::string state;
    std::string postalCode;
};

struct PhoneNumber
{
    enum Type {
        home, office, mobile
    } type;
    std::string number;
};

struct PersonBase
{
    std::string firstName;
    std::string lastName;
    bool isAlive;
    unsigned age;
    Address address;
    std::vector<PhoneNumber> phoneNumbers;
};

struct Person : public PersonBase
{
    std::vector<PersonBase> children;
    boost::optional<PersonBase> spouse;
};

// serialization operators

template <typename T>
void operator<<= (cxxtools::SerializationInfo& si, const boost::optional<T>& o)
{
    if (o)
        si <<= *o;
}

void operator<<= (cxxtools::SerializationInfo& si, const Address& a)
{
    si.addMember("streetAddress") <<= a.streetAddress;
    si.addMember("city") <<= a.city;
    si.addMember("state") <<= a.state;
    si.addMember("postalCode") <<= a.postalCode;
}

void operator<<= (cxxtools::SerializationInfo& si, const PhoneNumber::Type& t)
{
    switch(t)
    {
        case PhoneNumber::home: si <<= "home"; break;
        case PhoneNumber::office: si <<= "office"; break;
        case PhoneNumber::mobile: si <<= "mobile"; break;
    }
}

void operator<<= (cxxtools::SerializationInfo& si, const PhoneNumber& n)
{
    si.addMember("type") <<= n.type;
    si.addMember("number") <<= n.number;
}

void operator<<= (cxxtools::SerializationInfo& si, const PersonBase& p)
{
    si.addMember("firstName") <<= p.firstName;
    si.addMember("lastName") <<= p.lastName;
    si.addMember("isAlive") <<= p.isAlive;
    si.addMember("age") <<= p.age;
    si.addMember("address") <<= p.address;
    si.addMember("phoneNumbers") <<= p.phoneNumbers;
}

void operator<<= (cxxtools::SerializationInfo& si, const Person& p)
{
    si <<= static_cast<const PersonBase&>(p);
    si.addMember("children") <<= p.children;
    si.addMember("spouse") <<= p.spouse;
}

// main
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
