#include "Webserv.hpp"
#include <bits/types/struct_timeval.h>
#include <csignal>
#include <cstring>
#include <fstream>
#include <iostream>
#include <Socket.hpp>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include "ServerConfig.hpp"
std::string default_response("\
HTTP/1.1 200 OK\n\
Content-Type: text/plain\n\
Content-Length: 12\n\n\
Hello world!");

//--------------Functions----------------//
void            sigint_handler(int signal);
char**          create_exec_args(const std::vector<std::string>& args);
void            free_exec_args(char** exec_args);
void            serverLoop(std::map<int, Webserv> map_serv);
void            closeFds(fd_set &set, int max_fd);
std::string     getRequest(int fd, fd_set& set);
int             fd_is_serv(int fd, std::map<int, Webserv> map_serv, fd_set& set);
volatile sig_atomic_t   flag = 0;

int     Webserv::getFd() const
{
    return this->_sock_serv.getFd(); 
}

int     Webserv::_executeCgi(int fd, std::string path, std::vector<std::string> args)
{
    char** tmp = create_exec_args(args);
    int status = 0;
    int child = 0;
//    int pipe_fd[2];
//    if (pipe(pipe_fd) == -1)
//    {
//    perror("pipe in executCgi");
//    exit(EXIT_FAILURE);
//    }
    child = fork();
    if (child == 0)
    {
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2 in child");
            exit(EXIT_FAILURE);
        }
        execve(path.c_str(), tmp, envp); 
        perror("execve");
        exit(1);
    }
    if (waitpid(child, &status, 0) == -1)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status))
    {
        std::cerr << "Child terminated abnormally" << std::endl;
        return -1;
    }
    free_exec_args(tmp);
    return 0;
}
void    Webserv::sendResponse(int fd, std::string response)
{
    /*      CGI TEST
    response = "ET OUI MEME T'ES BIEN MOUCHEE";
    std::string                 path("cgi/bin/cgi_test.cgi");
    std::vector<std::string>    args;
    args.push_back("cgi_test.cgi");
    _executeCgi(fd, path, args);
    */
    std::ifstream       in("test.html");
    std::stringstream   buff;
    buff << in.rdbuf();
    in.close();

    response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    response += buff.str();
    if ((write(fd, response.c_str(), response.size())) == -1)
    {
        perror("write in send_response");
        exit(EXIT_FAILURE);
    }
}
//--------------Operators----------------//
Webserv&	Webserv::operator=(Webserv const&  rhs)
{
	if (this != &rhs)
	{
		_config = rhs._config;
        _sock_serv = rhs._sock_serv;
        envp = rhs.envp;
	}
	return *this;
}
//--------------Constructors------------//
Webserv::Webserv(ServerConfig &new_config) {
	_config = new_config;
}
Webserv::Webserv(int port) : _sock_serv(port)
{
    if (listen(_sock_serv.getFd(), 1) < 0)
    {
        perror("cannot listen");
        exit(1);
    }
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
void    Webserv::serverLoop(std::map<int, Webserv> map_serv)
{
    struct timeval          timeout;
    std::string             request;
    Socket                  tmp_socket;
    fd_set                  original_set;
    fd_set                  tmp_fd_set;
    ssize_t                 tmp_ret_value;
    int                     max_fd = 0;
    std::map<int, Socket>   sock_clients;

    FD_ZERO(&original_set);
    for (std::map<int, Webserv>::iterator it = map_serv.begin(); it != map_serv.end(); it++)
    {
        FD_SET(it->first, &original_set);
        max_fd = it->first;
    }

    signal(SIGINT, sigint_handler);
    while (!flag)
    {
        tmp_fd_set = original_set;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        std::cout << "\tWaiting for something to happen.."<<std::endl;
        tmp_ret_value = select(max_fd + 1, &tmp_fd_set, NULL, NULL, &timeout);
        if (tmp_ret_value < 0)
        {
            perror("select:");
            exit(EXIT_FAILURE);
        }
        if (tmp_ret_value == 0)
            continue;
        for (int i = 0; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &tmp_fd_set))
            {
                if (fd_is_serv(i, map_serv, tmp_fd_set))
                {
                    Socket::Accept(map_serv[i]._sock_serv, tmp_socket);
                    sock_clients.insert(std::pair<int, Socket>(tmp_socket.getFd(), tmp_socket));
                    tmp_socket.showInfo();
                    FD_SET(tmp_socket.getFd(), &original_set);
                    if (tmp_socket.getFd() > max_fd)
                        max_fd = tmp_socket.getFd();
                }
                else
                {
                    if ((request = getRequest(i, original_set)).empty())
                        continue;
                    //TODO Process request;
                    std::cout << request << std::endl;
                    map_serv[sock_clients[i].server_fd].sendResponse(i, default_response);
                    sock_clients[i].showInfo();
                }
            }
            if (flag)
                break;
        }
    }
    closeFds(original_set, max_fd);
}
void    Webserv::closeFds(fd_set& set, int max_fd)
{
    for (int i = 0; i < max_fd; i++)
    {
        if (FD_ISSET(i, &set))
        {
            close(i);
            FD_CLR(i, &set);
        }
    }
}
std::string Webserv::getRequest(int fd, fd_set& set)
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
        FD_CLR(fd, &set);
        return std::string("");
    }
    return ret;
}
char** create_exec_args(const std::vector<std::string>& args)
{
    char** exec_args = reinterpret_cast<char**>(malloc(sizeof(char*) * (args.size() + 1)));
    if (!exec_args)
        return NULL;
    for (size_t i = 0; i < args.size(); ++i)
    {
        exec_args[i] = strdup(args[i].c_str());
        if (!exec_args[i])
        {
            for (size_t j = 0; j < i; ++j)
                free(exec_args[j]);
            free(exec_args);
            return NULL;
        }
    }
    exec_args[args.size()] = NULL;
    return exec_args;
}
int fd_is_serv(int fd, std::map<int, Webserv> map_serv, fd_set& set)
{
    int last = map_serv.rbegin()->first;
    for (size_t i = 0; i < map_serv.size(); i++)
    {
        if (fd == map_serv[i].getFd() && FD_ISSET(fd, &set))
            return 1;
        if (fd > last)
            return 0;
    }
    return 0;
}
void free_exec_args(char** exec_args)
{
    if (!exec_args)
        return;
    for (char** p = exec_args; *p; ++p)
        free(*p);
    free(exec_args);
}
void    sigint_handler(int signal)
{
    std::cout << "Stopping server, sigint:" << signal<<std::endl;
    flag = 1;
}
