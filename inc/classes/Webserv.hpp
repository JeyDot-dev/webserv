#ifndef		WEBSERV_HPP
# define	WEBSERV_HPP
#  ifndef BUFFER_SIZE
#   define BUFFER_SIZE 1024
#  endif
#define TIMEOUT_SEC 5
#include <iostream>
#include <sys/select.h>
#include <vector>
#include <map>
#include "Socket.hpp"
#include "ServerConfig.hpp"
#include <string>
class	Webserv
{
public:
    int                 getFd() const;
    void                sendResponse(int fd, std::string response);

	Webserv&        operator=(Webserv const& rhs);

	Webserv(void);
    Webserv(int port);
	Webserv(ServerConfig &new_config);
	Webserv(Webserv const & src);
	~Webserv(void);

    char**                  envp;

private:
	ServerConfig            _config;
    //Response stuff
    int                     _executeCgi(int fd, std::string path, std::vector<std::string> args);
    //Socket/ Client connect, Accept, Respond--

    Socket                  _sock_serv;
    //------------------------------------------
public:
    //-------------NON MEMBER-------------------
    static void         serverLoop(std::map<int, Webserv> map_serv);
    static void         closeFds(fd_set &set, int max_fd);
    static std::string  getRequest(int fd, fd_set& set);
};

#endif
