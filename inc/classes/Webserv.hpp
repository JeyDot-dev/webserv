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
class	Webserv
{

public:
    void    serverLoop();
    Webserv(int port);
	Webserv(void);
	~Webserv(void);

    char**                  envp;

private:
    //Response stuff
    int                     _executeCgi(int fd, std::string path, std::vector<std::string> args);
    //Socket/ Client connect, Accept, Respond--
    std::string             _getRequest(int fd);
    void                    _closeFds();
    void                    _sendResponse(int fd, std::string response);
    Socket                  _sock_serv;
    std::map<int, Socket>   _sock_clients;
    fd_set                  _client_fd_set;
    int                     _max_fd;
    std::string             _default_response;
    //------------------------------------------

	Webserv(Webserv const & src);
	Webserv&	operator=(Webserv const& rhs);
};

#endif
