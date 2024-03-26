#include "ServerConfig.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void ServerConfig::init(std::string file_path)
{
	std::ifstream file(file_path.c_str());
    if (!file)
        throw std::runtime_error("Cannot open file: " + file_path);

    std::string line;
    std::string current_location;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        iss >> word;

        if (word == "server") {
            // Début d'un bloc de serveur
        } else if (word == "listen") {
            iss >> _port;
        } else if (word == "server_name") {
            iss >> _server_name;
        } else if (word == "location") {
            iss >> current_location;
        } else if (word == "root" || word == "access" || word == "method") {
            std::string value;
            iss >> value;
			if (value.back() == ';')
				value.pop_back();
            _locations[current_location][word] = value;
        } else if (word == "}") {
            // Fin d'un bloc
            if (!current_location.empty())
                current_location.clear();
        }
    }
    file.close();
}

int ServerConfig::getPort() const
{
	return _port;
}

std::string ServerConfig::getServerName() const
{
	return _server_name;
}

std::map<std::string, std::map<std::string, std::string> > ServerConfig::getLocations() const
{
	return _locations;
}

std::map<std::string, std::string> ServerConfig::getLocation(std::string location) const
{
	try {
        return _locations.at(location);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Location not found: " + location);
    }
}

std::string ServerConfig::getLocationValue(std::string location, std::string key) const
{
    try {
        return _locations.at(location).at(key);
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Key not found: " + key);
    }
}

//CANON:
ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig& src)
{
	*this = src;
	std::cout << "ServerConfig copy constructor called" << std::endl;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_server_name = rhs._server_name;
		_locations = rhs._locations;
	}
	std::cout << "ServerConfig assignment operator called" << std::endl;
	return *this;
}
