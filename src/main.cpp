#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include <cstdlib>

std::map<std::string, Webserv>	configFileParser(std::string filename);

void getPingRequest(Request req, int fd)
{
	std::cout << "Ping request received" << std::endl;
	std::cout << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req.headers.begin(); it != req.headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	res("200 OK", "text/html", "<html><body><h1>Pong</h1></body></html>", fd);
}

int main(int ac, char **av, char** envp)
{
	(void)av;
	(void)envp;
	if (ac != 2)
	{
		std::cerr << "Usage: ./Webserv <config_file>" << std::endl;
		return 1;
	}

	ServerConfig config;

	try
	{
		config.init(av[1]);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	Webserv server(config);

	server.serverLoop();

	// Webserv server(8080);

	// server.use("/", "./site/");
	// server.use("/CSS/", "./site/CSS/");
	// server.use("/IMG/", "./site/IMG/");

	// server.get("/api/ping", getPingRequest);

    // server.envp = envp;    
    // server.serverLoop();

    return 0;
}
