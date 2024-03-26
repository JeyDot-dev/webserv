#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "ServerConfig.hpp"

class Webserv {

	private:
		ServerConfig	_config;

	public:
		Webserv(void);
		Webserv(Webserv const &other);
		Webserv(ServerConfig &new_config);
		~Webserv(void);
		Webserv	&operator=(Webserv const &other);


};
