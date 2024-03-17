#include "Webserv.hpp"
#include "ServerConfig.hpp"

Webserv::Webserv(void)  {

}

Webserv::Webserv(Webserv const &other) : _config(other._config) {

}

Webserv::Webserv(ServerConfig &new_config) {
	_config = new_config;
}

Webserv::~Webserv(void) {

}

Webserv	&Webserv::operator=(Webserv const &other) {
	if (this != &other) {
		_config = other._config;
	}
	return *this;
}
