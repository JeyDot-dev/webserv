#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include "utils.hpp"

int main(int ac, char **av)
{
    std::map<int, Webserv> map_serv;

	if (ac != 2)
	{
		std::cerr << "Usage: ./Webserv <config_file>" << std::endl;
		return 1;
	}

	webservInit(av[1], map_serv);

	Webserv::serverLoop(map_serv);

    return 0;
}
