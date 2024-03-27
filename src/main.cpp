#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void getPingRequest(Request req, int fd)
{
	std::cout << "Ping request received" << std::endl;
	std::cout << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = req.headers.begin(); it != req.headers.end(); it++)
		std::cout << it->first << ": " << it->second << std::endl;
	res("200 OK", "text/html", "<html><body><h1>Pong</h1></body></html>", fd);
}

std::map<int, Webserv> initMapServ(std::string file_path)
{
	std::map<int, Webserv> res;

	std::ifstream file(file_path.c_str());
	if (!file)
		throw std::runtime_error("Cannot open file: " + file_path);

	std::string line;
    std::string current_location;

	ServerConfig actual_config;

	while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "server") {
            // Début d'un bloc de serveur
        } else if (word == "listen") {
			int _port;
            iss >> _port;
			actual_config.setPort(_port);
        } else if (word == "server_name") {
			std::string _server_name;
            iss >> _server_name;
			actual_config.setServerName(_server_name);
        } else if (word == "location") {
            iss >> current_location;
        } else if (word == "root" || word == "access" || word == "method") {
            std::string value;
            iss >> value;
			if (value[value.size() - 1] == ';')
				value.erase(value.size() - 1);
			actual_config.setLocationValue(current_location, word, value);
        } else if (word == "}") {
            if (!current_location.empty())
                current_location.clear();
			else {
				Webserv server(actual_config); // Nouveau serveur avec la nouvelle configuration
				res[server.getFd()] = server; // Ajout du serveur à la map
				std::cout << "New server: " << actual_config.getServerName() << std::endl; // On le dit
				actual_config = ServerConfig(); // On réinitialise la configuration pour le prochain serveur
			}
	    }
    }

	file.close();
	return res;
}

void webservInit(std::string file_path, std::map<int, Webserv> &map_serv)
{
	try
	{
		map_serv = initMapServ(file_path);
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		exit(1);
	}
}

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

	// Webserv server(8080);

	// server.use("/", "./site/");
	// server.use("/CSS/", "./site/CSS/");
	// server.use("/IMG/", "./site/IMG/");

	// server.get("/api/ping", getPingRequest);

    // server.envp = envp;    
    // server.serverLoop();

    return 0;
}
