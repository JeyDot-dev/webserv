#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include <cstdlib>

std::map<std::string, Webserv>	configFileParser(std::string filename);
int main(int ac, char **av, char** envp)
{
    std::map<int, Webserv> map_serv;
	if (ac != 2)
		std::cerr << "Usage: ./Webserv <config_file>" << std::endl;

	configFileParser(av[1]);
    for (int i = 8080; i < 8090; i++)
    {
        Webserv tmp(i);
        tmp.envp = envp;
        map_serv.insert(std::pair<int, Webserv>(tmp.getFd(), tmp));
    }

    Webserv::serverLoop(map_serv);

    return 0;
}
