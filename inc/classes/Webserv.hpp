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
	std::map<std::string, std::string> &static_folders;
	fd_set			&set;

	Request(std::map<std::string, std::string> &static_folders, fd_set &set) : static_folders(static_folders), set(set) {}
};

typedef void (*FunctionType)(Request, int);

class	Webserv
{
public:
    int                 getFd() const;
    void				sendResponse(int fd, Request req);

	Webserv&        operator=(Webserv const& rhs);

	Webserv(void);
    Webserv(int port);
	Webserv(ServerConfig &new_config);
	Webserv(Webserv const & src);
	~Webserv(void);

	void use(std::string path, std::string folder);
	void get(std::string path, FunctionType func);
	void getResponse(Request req, int fd);
	void post(std::string path, FunctionType func);
	void postResponse(Request req, int fd);

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
	std::map<std::string, std::string> _static_folders;
	std::map<std::string, FunctionType> _get;
	std::map<std::string, FunctionType> _post;
	std::vector<Request>	_requests;

    //------------------------------------------
public:
    //-------------NON MEMBER-------------------
    static void         serverLoop(std::map<int, Webserv> map_serv);
    static void         closeFds(fd_set &set, int max_fd);
    static std::string  getRequest(int fd, fd_set& set);
};

void send_file(int fd, const std::string& path, const std::string& mime_type);
bool file_exists(const std::string& filename);
void res(std::string status, std::string headers, std::string body, int fd);
void res(std::string rep, int fd);


#endif
