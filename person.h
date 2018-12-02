#ifndef PERSON_H
#define PERSON_H

#include <cxxtools/serializationinfo.h>
#include <cxxtools/serializationerror.h>
#include <boost/optional.hpp>

template <typename T>
void operator<<= (cxxtools::SerializationInfo& si, const boost::optional<T>& o)
{
    if (o)
        si <<= *o;
}

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

inline void operator<<= (cxxtools::SerializationInfo& si, const Address& a)
{
    si.addMember("streetAddress") <<= a.streetAddress;
    si.addMember("city") <<= a.city;
    si.addMember("state") <<= a.state;
    si.addMember("postalCode") <<= a.postalCode;
}

inline void operator>>= (const cxxtools::SerializationInfo& si, Address& a)
{
    si.getMember("streetAddress") >>= a.streetAddress;
    si.getMember("city") >>= a.city;
    si.getMember("state") >>= a.state;
    si.getMember("postalCode") >>= a.postalCode;
}

inline void operator<<= (cxxtools::SerializationInfo& si, const PhoneNumber::Type& t)
{
    switch(t)
    {
        case PhoneNumber::home: si <<= "home"; break;
        case PhoneNumber::office: si <<= "office"; break;
        case PhoneNumber::mobile: si <<= "mobile"; break;
    }
}

inline void operator>>= (const cxxtools::SerializationInfo& si, PhoneNumber::Type& t)
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

inline void operator<<= (cxxtools::SerializationInfo& si, const PhoneNumber& n)
{
    si.addMember("type") <<= n.type;
    si.addMember("number") <<= n.number;
}

inline void operator>>= (const cxxtools::SerializationInfo& si, PhoneNumber& n)
{
    si.getMember("type") >>= n.type;
    si.getMember("number") >>= n.number;
}

inline void operator<<= (cxxtools::SerializationInfo& si, const PersonBase& p)
{
    si.addMember("firstName") <<= p.firstName;
    si.addMember("lastName") <<= p.lastName;
    si.addMember("isAlive") <<= p.isAlive;
    si.addMember("age") <<= p.age;
    si.addMember("address") <<= p.address;
    si.addMember("phoneNumbers") <<= p.phoneNumbers;
}

inline void operator>>= (const cxxtools::SerializationInfo& si, PersonBase& p)
{
    si.getMember("firstName") >>= p.firstName;
    si.getMember("lastName") >>= p.lastName;
    si.getMember("isAlive") >>= p.isAlive;
    si.getMember("age") >>= p.age;
    si.getMember("address") >>= p.address;
    si.getMember("phoneNumbers") >>= p.phoneNumbers;
}

inline void operator<<= (cxxtools::SerializationInfo& si, const Person& p)
{
    si <<= static_cast<const PersonBase&>(p);
    si.addMember("children") <<= p.children;
    si.addMember("spouse") <<= p.spouse;
}

inline void operator>>= (const cxxtools::SerializationInfo& si, Person& p)
{
    si >>= static_cast<PersonBase&>(p);
    si.getMember("children") >>= p.children;
    si.getMember("spouse") >>= p.spouse;
}

#endif
