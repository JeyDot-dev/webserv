#include "Webserv.hpp"
#include <cstdlib>
int main(int ac, char **av, char** envp)
{
    if (ac != 2)
    {
        std::cout << "./webserv port" <<std::endl;
        return 1;
    }
    Webserv server(atoi(av[1]));

    server.envp = envp;    
    server.serverLoop();

    return 0;
}
