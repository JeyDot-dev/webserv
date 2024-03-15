#include "Webserv.hpp"
#include <bits/types/struct_timeval.h>
#include <iostream>
#include <Socket.hpp>
#include <cstdio>
#include <cstdlib>
#include <sys/select.h>
#include <unistd.h>
#include <map>

//--------------Functions----------------//
void    Webserv::_closeFds()
{
    for (int i = 0; i < _max_fd; i++)
    {
        if (FD_ISSET(i, &_client_fd_set))
            close(i);
    }
}
void    Webserv::_sendResponse(int fd, std::string response)
{
    if ((write(fd, response.c_str(), response.size())) == -1)
    {
        perror("write in send_response");
        exit(EXIT_FAILURE);
    }
    _sock_clients[fd].showInfo();
}
std::string Webserv::_getRequest(int fd)
{
    int         bytes_read = 0;
    char        buffer[BUFFER_SIZE];
    std::string ret;

    if ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        ret.append(buffer, bytes_read);
    else if (bytes_read == -1)
    {
        perror("read in get_request");
        exit(EXIT_FAILURE);
    }
    else
    {
        close(fd);
        FD_CLR(fd, &_client_fd_set);
        return std::string("");
    }
    return ret;
}
void    Webserv::serverLoop()
{
    struct timeval  timeout;
    std::string     request;
    Socket          tmp_socket;
    fd_set          tmp_fd_set;
    ssize_t         tmp_ret_value;
    while (true)
    {
        tmp_fd_set = _client_fd_set;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        std::cout << "\tWaiting for something to happen.."<<std::endl;
        tmp_ret_value = select(_max_fd + 1, &tmp_fd_set, NULL, NULL, &timeout);
        if (tmp_ret_value < 0)
        {
            perror("select:");
            exit(EXIT_FAILURE);
        }
        if (tmp_ret_value == 0)
            break;
        for (int i = 0; i <= _max_fd; i++)
        {
            if (FD_ISSET(i, &tmp_fd_set))
            {
                if (i == _sock_serv.getFd())
                {
                    Socket::Accept(_sock_serv, tmp_socket);
                    _sock_clients.insert(std::pair<int, Socket>(tmp_socket.getFd(), tmp_socket));
                    tmp_socket.showInfo();
                    FD_SET(tmp_socket.getFd(), &_client_fd_set);
                    if (tmp_socket.getFd() > _max_fd)
                        _max_fd = tmp_socket.getFd();
                }
                else
                {
                    if ((request = _getRequest(i)).empty())
                        continue;
                    //TODO Process request;
                    //std::cout << http_request << std::endl;
                    _sendResponse(i, _default_response);
                }
            }
        }
    }
    _closeFds();
}
//--------------Operators----------------//
Webserv&	Webserv::operator=(Webserv const&  rhs)
{
	if (this != &rhs)
	{
		//copy
	}
	return *this;
}
//--------------Constructors------------//
Webserv::Webserv(int port) : _sock_serv(port), _default_response("\
HTTP/1.1 200 OK\n\
Content-Type: text/plain\n\
Content-Length: 12\n\n\
Hello world!")
{
    if (listen(_sock_serv.getFd(), 1) < 0)
    {
        perror("cannot listen");
        exit(1);
    }
    _max_fd = _sock_serv.getFd();
    FD_ZERO(&_client_fd_set);
    FD_SET(_sock_serv.getFd(), &_client_fd_set);
}
Webserv::Webserv(Webserv const &src)
{
	*this = src;
	return ;
}

Webserv::Webserv(void)
{
	return ;
}

Webserv::~Webserv(void)
{
	return ;
}
//--------------Non-Member--------------//
