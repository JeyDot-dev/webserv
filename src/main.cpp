#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include <cstdlib>

std::map<std::string, Webserv>	configFileParser(std::string filename);
int main(int ac, char **av, char** envp)
{
	if (ac != 2)
		std::cerr << "Usage: ./Webserv <config_file>" << std::endl;

	configFileParser(av[1]);
    Webserv server(8080);

	server.use("/", "./site/");
	server.use("/CSS/", "./site/CSS/");
	server.use("/IMG/", "./site/IMG/");
	//server.use("/JS", "./site/JS/");

    server.envp = envp;    
    server.serverLoop();

    return 0;
}
