#include <iostream>
#include <tnt/tntnet.h>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>

int main(int argc, char* argv[])
{
    try
    {
        log_init();
        cxxtools::Arg<unsigned short> port(argc, argv, 'p', 8000);

        tnt::Tntnet app;
        app.listen(port);

        app.mapUrl("^/(.*)", "$1");
        app.mapUrl("^/(.*).json", "json/$1");
        app.mapUrl("^/$", "webmain");
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

