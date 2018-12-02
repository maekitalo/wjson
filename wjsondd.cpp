#include <cxxtools/argin.h>
#include <cxxtools/json.h>
#include <cxxtools/serializationinfo.h>
#include <cxxtools/serializationerror.h>
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

// deserialization operators

template <typename T>
void operator>>= (const cxxtools::SerializationInfo& si, boost::optional<T>& o)
{
    if (!si.isNull())
    {
        T v;
        si >>= v;
        o = boost::optional<T>(std::move(v));
    }
}

void operator>>= (const cxxtools::SerializationInfo& si, Address& a)
{
    si.getMember("streetAddress") >>= a.streetAddress;
    si.getMember("city") >>= a.city;
    si.getMember("state") >>= a.state;
    si.getMember("postalCode") >>= a.postalCode;
}

void operator>>= (const cxxtools::SerializationInfo& si, PhoneNumber::Type& t)
{
    std::string v;
    si >>= v;
    if (v == "home")
        t = PhoneNumber::home;
    else if (v == "office")
        t = PhoneNumber::office;
    else if (v == "mobile")
        t = PhoneNumber::mobile;
    else
        throw cxxtools::SerializationError("invalid phone number type \"" + v + '"');
}

void operator>>= (const cxxtools::SerializationInfo& si, PhoneNumber& n)
{
    si.getMember("type") >>= n.type;
    si.getMember("number") >>= n.number;
}

void operator>>= (const cxxtools::SerializationInfo& si, PersonBase& p)
{
    si.getMember("firstName") >>= p.firstName;
    si.getMember("lastName") >>= p.lastName;
    si.getMember("isAlive") >>= p.isAlive;
    si.getMember("age") >>= p.age;
    si.getMember("address") >>= p.address;
    si.getMember("phoneNumbers") >>= p.phoneNumbers;
}

void operator>>= (const cxxtools::SerializationInfo& si, Person& p)
{
    si >>= static_cast<PersonBase&>(p);
    si.getMember("children") >>= p.children;
    si.getMember("spouse") >>= p.spouse;
}

// main
int main(int argc, char* argv[])
{
    try
    {
        cxxtools::ArgIn in(argc, argv);

        Person p;
        in >> cxxtools::Json(p);

        std::cout << "first name: " << p.firstName << "\n"
                     "last name: " << p.lastName << "\n"
                     "age: " << p.age << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
