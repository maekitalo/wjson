JSON with cxxtools
==================

Cxxtools is a extensive c++ library, which has a serialization framework for
various formats. One of those is [JSON](https://www.json.org/). Here we describe
how to use JSON with cxxtools.

The serialization framework uses a intermediate class
`cxxtools::SerializationInfo` to convert between different formats. This
collects the structure of objects or values.

A serialization info object can contain:

 * a single value
 * a object
 * a array
 * or can be just empty

The type of the object is specified by a category. Most of the time the category
is set automatically. When the value is set, the category is set to value. When
sub members are added, the category is set to object when the member has a name
or to array when no name is given. So that normally there is no need to set the
category.

Cxxtools uses the operator `<<=` to serialize data. The left operand is a
reference to a `cxxtools::SerializationInfo` and the right operand a constant
reference to the value to be serialized. Cxxtools provides this serialization
operator for all standard C++ types. The use can define the operator for own
types when needed. We will use that later.

The operator `>>=' is the opposite. It expects a constant reference to a
`cxxtools::SerializationInfo` as the left operand and a non constant reference
to a object to be read.

So to create a JSON structure, we need to create a `cxxtools::SerializationInfo`
first.

Simple example
--------------

We start with a simple example. We write a JSON object to standard output. First
we need to create a `cxxtools::SerializationInfo` and fill it with some data.

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

Write the content to _wjson.cpp_ and compile it with (we assume, you have
cxxtools installed in your system):

    g++ -o wjson -lcxxtools wjson.cpp

Executing the program results in a JSON output:

    {
        "firstName": "John",
        "lastName": "Smith",
        "age": 27
    }

We created a  `cxxtools::SerializationInfo` and filled it with some data. The
member function `addMember` creates a sub object in our serialization info
object.

The function `cxxtools::Json` creates an helper object, which has a output
operator to std::ostream, which generates the JSON. The method `beautify`
enables formatting of the object. Since it returns the same helper object back
it can be chained like shown here.

Note that it is always a good idea to catch exceptions.

Now we want to read the data back. Here is the code:

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

This reads a JSON object from standard input and outputs the attributes.
Write the content to _wjsond.cpp_ and compile it with

    g++ -o wjsond -lcxxtools wjsond.cpp

The helper object returned by `cxxtools::Json` has also a input operator, which
reads JSON from a `std::istream` and fills the passed object with it.

Now you can chain the 2 programs together by running:

    ./wjson | ./wjsond

Now the output of `wjson` is sent to the input of `wjsond`. The result is this:

    first name: John
    last name: Smith
    age: 27

Not really a big surprise.

Bigger example
--------------

Lets assume, we want to create this JSON object (see
[wikipedia](https://en.wikipedia.org/wiki/JSON)):

    {
      "firstName": "John",
      "lastName": "Smith",
      "isAlive": true,
      "age": 27,
      "address": {
        "streetAddress": "21 2nd Street",
        "city": "New York",
        "state": "NY",
        "postalCode": "10021-3100"
      },
      "phoneNumbers": [
        {
          "type": "home",
          "number": "212 555-1234"
        },
        {
          "type": "office",
          "number": "646 555-4567"
        },
        {
          "type": "mobile",
          "number": "123 456-7890"
        }
      ],
      "children": [],
      "spouse": null
    }

We start with a simple program, which creates the `cxxtools::SerializationInfo`
manually:

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

There are also some small features added.

The `cxxtools::Arg<bool>` checks for a command line option. In this case it
checks, for a option `-b` to control whether we want beautified output. The
class `cxxtools::ArgOut` defines a output stream. Writing to that writes either
to a file, which is fetched from the command line or if no argument is found,
standard output is used. Note that `cxxtools::Arg<bool>` removes that -b from
the command line if found.

Putting that class into a file _wjsonc.cpp_ and compiling with

    g++ -o wjsonc -lcxxtools wjsonc.cpp

creates our program. Running it with _./jsonc -b_ outputs the above structure.
Without -b the output is also valid JSON without any whitespace, which is hard
to read for humans but compact and easy to read with a JSON deserializer.

The above `jsond` can be used to read the structure as well. You can extend that
with the additional attributes if you want but we will do it later with a better
approach.

### Overriding serialization operator

Now it is time to improve our serialization. It is quite difficult to track,
which data belongs where. As described the serialization operator `<<=` is used
for serialization. And this operator can be overridden for other types.

First we will create C++ structures for the objects we need:

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

We use `boost::optional` here to model a optional member.

Next we can define serialization operators for all used types, which are not
standard types:

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

Converting an object to JSON is now much nicer:

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

We omit the whole program for brevity. You can download the complete examples at
the [tntnet](http://www.tntnet.org/) homepage.

To read the JSON structure back we will define deserialization operators for all
used types:

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

This makes it easy to read the data back:

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

We use the class `cxxtools::ArgIn` which is somewhat the opposite of
`cxxtools::ArgOut`. It defines a input stream from a file if a file name is
given on the command line or standard input otherwise.

Now we can again chain the programs together. We assume, we have created a
program _wjsoncc_ to create the JSON object and _wjsondd_ to read the data back.

    ./wjsoncc | ./wjsondd

Or alternatively you can create a json file using _./wjsoncc foo.json_ and read
it back with _./wjsondd foo.json_.

Using Json in tntnet
--------------------

### Requesting objects from JavaScript

JSON is often used in web application due to the affinity to JavaScript. Tntnet
is a web application server for C++, which uses cxxtools. So if you use Tntnet
you use cxxtools already.

Reading and writing JSON objects in JavaScript is easy. We use
[jQuery](https://jquery.com/) for that.

What we want is to process our application data in our web application. With
our JSON serialization it is easy to transport application data from Tntnet to
our client web application and back to the back end.

We start with sending objects to the client. Or better by requesting objects
from the server using jQuery.

We will use the above Person object in our example. Or a reduced version of
that for simplicity.

Our javascript code is here:

    $.getJSON('person.json', { }, function(p) {
        $('.firstName').val(p.firstName);
        $('.lastName').val(p.lastName);
        $('.age').val(p.age);
    });

This executes a json request and expects our Person class back. Now we create
our server side `person.ecpp`:

    <%pre>

    #include <cxxtools/json.h>

    namespace
    {
        struct Person
        {
            std::string firstName;
            std::string lastName;
            unsigned age;
        };

        void operator<<= (cxxtools::SerializationInfo& si, const Person& p)
        {
            si.addMember("firstName") <<= p.firstName;
            si.addMember("lastName") <<= p.lastName;
            si.addMember("age") <<= p.age;
        }
    }

    </%pre>
    <%cpp>

    Person p;
    p.firstName = "John";
    p.lastName = "Smith";
    p.age = 27;

    reply.out() << cxxtools::Json(p);
    reply.setContentType("text/json");

    </%cpp>

When this is mapped to the url _person.json_ it will now send the person to the
browser if requested.

The full example can be found in the download package.

### Sending objects to the server

A quite new feature in tntnet is to be able to parse data sent by jQuery using
serialization. This is how.

First let us again look to the client side in JavaScript:

    $.getJSON(
        'storePerson.json',
        {
            person: {
                firstName: "John",
                lastName: "Smith",
                age: 27
            }
        },
        function(reply) {
            // we actually do not need a reply in this example
        });

To read that person on the serve side we have this ecpp:

    <%pre>

    #include <cxxtools/json.h>

    namespace
    {
        struct Person
        {
            std::string firstName;
            std::string lastName;
            unsigned age;
        };

        void operator>>= (const cxxtools::SerializationInfo& si, Person& p)
        {
            si.getMember("firstName") >>= p.firstName;
            si.getMember("lastName") >>= p.lastName;
            si.getMember("age") >>= p.age;
        }
    }

    </%pre>
    <%cpp>

    cxxtools::SerializationInfo si;
    si <<= qparams;

    Person person;
    si.getMember("person") >>= person;

    std::cout << "first name: " << person.firstName << "\n"
                 "last name: " << person.lastName << "\n"
                 "age: " << person.age << "\n";

    // we send just an empty json reply back
    reply.out() << "{}";
    reply.setContentType("text/json");

    </%cpp>

Or just even nicer (with the even more recent tntnet):

    <%args>
    Person person;
    </%args>
    <%cpp>

    std::cout << "first name: " << person.firstName << "\n"
                 "last name: " << person.lastName << "\n"
                 "age: " << person.age << "\n";

    // we send just an empty json reply back
    reply.out() << "{}";
    reply.setContentType("text/json");

    </%cpp>

Of course printing to `std::cout` is somewhat useless for our web server but it
is here just as an example, that we can do something with that.

### Unicode

You may have seen that there is a problem with unicode in the above tntnet
example. If you enter a non name with a ascii character (like my last name
'Mäkitalo') and press the store button, tntnet outputs looks ugly.

The problem is, that `std::string` is not unicode safe. Its characters
are just 8 bit and this is not enough for unicode strings.

There are 2 possible solutions, cxxtools provides.

We can use either Utf-8, which fits into a `std::string` or use the cxxtools
unicode string `cxxtools::String`.

Cxxtools provides a Utf-8 encoder and decoder, which is easy to use in the
serialization framework. We need to include the header `cxxtools/utf8.h` and
change the serializer and deserializer of our Person class.

        void operator<<= (cxxtools::SerializationInfo& si, const Person& p)
        {
            si.addMember("firstName") <<= cxxtools::Utf8(p.firstName);
            si.addMember("lastName") <<= cxxtools::Utf8(p.lastName);
            si.addMember("age") <<= p.age;
        }

        void operator>>= (const cxxtools::SerializationInfo& si, Person& p)
        {
            si.getMember("firstName") >>= cxxtools::Utf8(p.firstName);
            si.getMember("lastName") >>= cxxtools::Utf8(p.lastName);
            si.getMember("age") >>= p.age;
        }

The helper function `cxxtools::Utf8` returns much like the Json function above a
wrapper class, which has a serialization and deserialization operator. And also
input and output operators for `std::iostream`, which we do not need here. Now
the first name we get from the browser, which is Utf-8 encoded is translated
into a unicode string for the `SerializationInfo`, which supports unicode.

The other solution is not to use std::string but `cxxtools::String` in our Person
class. Replacing std::string in Person solves the problem.

The `cxxtools::String` has all the methods of a `std::string`. It is actually a
`std::basic_string<cxxtools::Char>`, where `cxxtools::Char` is a unicode
character.

It has also a std::ostream operator, which outputs Utf-8 since most output
devices work with that.

I recommend the latter solution since it is more correct. Note that you can
access a string by character but if you have a Utf-8 encoded data in a
`std::string` it is somewhat useless. It does not make any sense to access a
utf-8 string byte by byte. If the `std::string` _lastName_ contains my last name
(Mäkitalo), then `lastName[1]` returns the first byte of the umlaut _a_. But if
it is a `cxxtools::String`, then it returns exactly the umlaut _a_.
