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
//--------------Functions----------------//
void    sigint_handler(int signal);
char**  create_exec_args(const std::vector<std::string>& args);
void    free_exec_args(char** exec_args);
volatile sig_atomic_t   flag = 0;

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
void    Webserv::_closeFds()
{
    for (int i = 0; i < _max_fd; i++)
    {
        if (FD_ISSET(i, &_client_fd_set))
        {
            close(i);
            FD_CLR(i, &_client_fd_set);
        }
    }
}
void    Webserv::_sendResponse(int fd, std::string response)
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

    signal(SIGINT, sigint_handler);
    while (!flag)
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
                    std::cout << request << std::endl;
                    _sendResponse(i, _default_response);
                }
            }
            if (flag)
                break;
        }
    }
    _closeFds();
}
//--------------Operators----------------//
Webserv&	Webserv::operator=(Webserv const&  rhs)
{
	if (this != &rhs)
	{
		_config = rhs._config;
        _sock_serv = rhs._sock_serv;
        _sock_clients = rhs._sock_clients;
        _client_fd_set = rhs._client_fd_set;
        _max_fd = rhs._max_fd;
        _default_response = rhs._default_response;
        envp = rhs.envp;
	}
	return *this;
}
//--------------Constructors------------//
Webserv::Webserv(ServerConfig &new_config) {
	_config = new_config;
}
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
