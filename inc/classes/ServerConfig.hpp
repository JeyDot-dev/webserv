#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <exception>

class ServerConfig {
private:
	int									_port;
	std::string							_server_name;
	std::map<std::string, std::map<std::string, std::string> >	_locations;
public:
	ServerConfig();
	ServerConfig(ServerConfig const &other);
	~ServerConfig();

	void		setPort(int port);
	void		setServerName(std::string server_name);

	void		setLocationValue(std::string location, std::string key, std::string value);

	int			getPort() const;
	std::string	getServerName() const;
	std::map<std::string, std::map<std::string, std::string> >	getLocations() const;
	std::map<std::string, std::string>	getLocation(std::string location) const;
	std::string	getLocationValue(std::string location, std::string key) const;

	ServerConfig	&operator=(ServerConfig const &other);
};
