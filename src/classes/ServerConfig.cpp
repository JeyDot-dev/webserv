#include "ServerConfig.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

void ServerConfig::setPort(int port)
{
	_port = port;
}

void ServerConfig::setServerName(std::string server_name)
{
	_server_name = server_name;
}

void ServerConfig::setLocationValue(std::string location, std::string key, std::string value)
{
	_locations[location][key] = value;
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
}

ServerConfig& ServerConfig::operator=(const ServerConfig& rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_server_name = rhs._server_name;
		_locations = rhs._locations;
	}
	return *this;
}
