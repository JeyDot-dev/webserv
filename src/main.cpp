#include "Webserv.hpp"
#include <cstdlib>
int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "./webserv port" <<std::endl;
        return 1;
    }
    Webserv                  server(atoi(av[1]));

    server.serverLoop();

    return 0;
}
