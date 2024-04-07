#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include "utils.hpp"

int main(int ac, char **av)
{
    std::map<int, Webserv> map_serv;

    if (ac == 2)
        webservInit(av[1], map_serv);
    else
        webservInit("server.conf", map_serv);

    Webserv::serverLoop(map_serv);
    return 0;
}
