#ifndef		WEBSERV_HPP
# define	WEBSERV_HPP
#  ifndef BUFFER_SIZE
#   define BUFFER_SIZE 1024
#  endif
#define TIMEOUT_SEC 8
#include <iostream>
#include <sys/select.h>
#include <vector>
#include <map>
#include "Socket.hpp"
#include "ServerConfig.hpp"
#include <string>

struct Response
{
	std::string     status;
	std::string     headers;
	std::string     body;
};

struct Request
{
	std::string     method;
	std::string     path;
	std::string     folder;
	std::string     file;
	std::string     mime_type;
	std::string     version;
	std::map<std::string, std::string> headers;
	std::string     body;
};

class	Webserv
{
public:
    void    serverLoop();
    
	Webserv&	operator=(Webserv const& rhs);

	Webserv(void);
    Webserv(int port);
	Webserv(ServerConfig &new_config);
	Webserv(Webserv const & src);
	~Webserv(void);

	void use(std::string path, std::string folder);

    char**                  envp;

private:
	ServerConfig            _config;
    //Response stuff
    int                     _executeCgi(int fd, std::string path, std::vector<std::string> args);
    //Socket/ Client connect, Accept, Respond--
    std::string             _getRequest(int fd);
    void                    _closeFds();
    void                    _sendResponse(int fd, std::string response, std::string path_file,  std::string file, std::string mime_type);

    Socket                  _sock_serv;
    std::map<int, Socket>   _sock_clients;
    fd_set                  _client_fd_set;
    int                     _max_fd;
    std::string             _default_response;
	std::map<std::string, std::string> _static_folders;
	std::vector<Request>	_requests;
    //------------------------------------------

};

#endif
