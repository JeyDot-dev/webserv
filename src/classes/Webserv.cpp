#include "Webserv.hpp"
//#include <bits/types/struct_timeval.h>
#include <sys/time.h>
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
#include <string>
#include "Cgi.hpp"
#include "ServerConfig.hpp"
#include "utils.hpp"
#include <algorithm>
#include <sys/stat.h>
#include <utils.hpp>

std::string default_response("\
HTTP/1.1 200 OK\n\
Content-Type: text/plain\n\
Content-Length: 12\n\n\
Hello world!");

std::string error_404("\
HTTP/1.1 404 Not Found\n\
Content-Type: text/html\n\n\
<html><body><h1>404 Not Found</h1></body></html>");

//--------------Functions----------------//
void            check_hanging(fd_set& read_set, fd_set& write_set, fd_set& tmp_read_set,
                                fd_set& tmp_write_set, std::map<int, Socket>& sock_clients, int max_fd,
                                std::map<int, Request>& map_req, std::map<int, Webserv>& map_serv);
void			sigint_handler(int signal);
char**		 	  create_exec_args(const std::vector<std::string>& args);
void			free_exec_args(char** exec_args);
void			serverLoop(std::map<int, Webserv> map_serv);
void			closeFds(fd_set &set, int max_fd);
std::string	 getRequest(int fd, fd_set& set);
int			 fd_is_serv(int fd, std::map<int, Webserv>& map_serv, fd_set& set);
volatile sig_atomic_t   flag = 0;
void send_file(int fd, const std::string& path, const std::string& mime_type);
bool file_exists(const std::string& filename);
void res(std::string status, std::string headers, std::string body, int fd);
void res(std::string rep, int fd);
void defaultPost(Request req, int fd);

std::string Webserv::getIp() const
{
    return this->_sock_serv.getIp();
}
int	 Webserv::getFd() const
{
	return this->_sock_serv.getFd(); 
}

bool directoryExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

std::string	 Webserv::_executeCgi(Request req, std::string client_ip, std::string host_ip)
{
    Cgi         cgi_class(req, client_ip, host_ip);
    std::string ret;
	char**      env = create_exec_args(cgi_class.getEnv());
	int         status = 0;
	int         child = 0;
    int         pipe_out[2];
    int         pipe_in[2];
    char**      args;

    if (cgi_class.getPath().empty() || (req.method == "POST" && pipe(pipe_in) == -1) || pipe(pipe_out) == -1)
	{
    	perror("execute Cgi");
        return ("Status: 500\r\n\r\n");
    }
    args = new char*[3];
    args[0] = strdup(cgi_class.getPath().c_str());
    args[1] = strdup(cgi_class.getScriptPath().c_str());
    args[2] = NULL;
    if ((child = fork()) == -1)
	{
        delete args[0];
        delete args;
    	perror("execute Cgi");
        return ("Status: 500\r\n\r\n");
    }
    if (req.method == "POST" && write(pipe_in[1], req.body.c_str(), req.body.size()) <= 0)
    {
        close(pipe_in[1]); close(pipe_in[0]); close(pipe_out[0]); close(pipe_out[1]);
        delete args[0];
        delete args;
        return ("Status: 422\r\n\r\n");
    }
    close(pipe_in[1]);
	if (child == 0)
	{
        close(pipe_out[0]);
		if ((req.method == "POST" && dup2(pipe_in[0], STDIN_FILENO) == -1) || dup2(pipe_out[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 in child");
			exit(EXIT_FAILURE);
		}
        close(pipe_in[0]);
        close(pipe_out[1]);
		execve(args[0], args, env); 
		perror("execve cgi:");
		exit(EXIT_FAILURE);
	}
    close(pipe_out[1]);
    close(pipe_in[0]);
	if (waitpid(child, &status, 0) == -1)
	{
        delete args[0];
        delete args;
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
    delete args[0];
    delete args;
	if (!WIFEXITED(status) || WEXITSTATUS(status))
	{
		std::cerr << "Child terminated abnormally" << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	free_exec_args(env);
    ret = read_from_pipe(pipe_out[0]);
    close(pipe_out[0]);
	return ret;
}

void	Webserv::sendResponse(int fd, Request req, std::string client_ip)
{
    //IF CGI
    //std::string response;
    std::string response = _executeCgi(req, client_ip, this->getIp());
    std::string response_test = "HTTP/1.1 200 OK\r\n" + response;
    std::cout << "RESPONSE FROM CGI: " << response_test << std::endl;
/*	if (req.method == "GET")
		getResponse(req, fd);
	else if (req.method == "POST")
		postResponse(req, fd);
	else*/
		res(response_test, fd);


// 	if (_static_folders.find(path) == _static_folders.end())
// 	{
// 		std::cerr << "Path not found:" << path << std::endl;
// 		return;
// 	}

//    std::string file_path = _static_folders[path] + file;
		
// 	if (_static_folders.find(path) == _static_folders.end()
// 		|| _static_folders[path].empty()
// 		|| mime_type.empty()
// 		|| !file_exists(file_path))
// 	{
// 		std::cerr << "Path empty:" << path << std::endl;
// 		write(fd, response.c_str(), response.size());
// 		return;
// 	}

// 	send_file(fd, file_path, mime_type);

// 	//_sock_clients[fd].showInfo();
	close(fd);
	FD_CLR(fd, req.set);
}

bool ends_with(const std::string& value, const std::string& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string getMimeType(const std::string& path)
{
	if (path.empty())
		return "text/html";
	if (ends_with(path, ".css"))
		return "text/css";
	if (ends_with(path, ".html"))
		return "text/html";
	if (ends_with(path, ".js"))
		return "text/javascript";
	if (ends_with(path, ".jpg") || ends_with(path, ".jpeg"))
		return "image/jpeg";
	if (ends_with(path, ".png"))
		return "image/png";
	return "text/plain";
}

void parseRequest(std::string request, Request &ret)
{
	//DEBUG:
	//std::cout << "Request: " << request << std::endl;

	ret.method = request.substr(0, request.find(' ')); //Par exemple GET
	request = request.substr(request.find(' ') + 1); //On enleve le GET
	ret.path = request.substr(0, request.find(' ')); //On recupere le path
	request = request.substr(request.find(' ') + 1); //On enleve le path
	ret.version = request.substr(0, request.find('\n')); //On recupere la version
	request = request.substr(request.find('\n') + 1); //On enleve la version
	while (request.find(":") != std::string::npos)
	{
		std::string key = request.substr(0, request.find(":"));
		request = request.substr(request.find(":") + 1);
		std::string value = request.substr(0, request.find("\n"));
		request = request.substr(request.find("\n") + 1);
		ret.headers.insert(std::pair<std::string, std::string>(key, value));
		std::cout << key << ": " << value << std::endl;
	}
	ret.body = request; //On recupere le body (c'est tout ce qui reste)

	ret.folder = ret.path.substr(0, ret.path.find_last_of('/'));
	ret.folder += "/";
	ret.file = ret.path.substr(ret.path.find_last_of('/') + 1);
	if (ret.file.empty())
		ret.file = "index.html";
	ret.mime_type = getMimeType(ret.file);

	//DEBUG:
	// std::cout << "Path: " << ret.path << std::endl;
	// std::cout << "Method: " << ret.mime_type << std::endl;
}

void Webserv::use(std::string path, std::string root)
{
	_static_folders.insert(std::pair<std::string, std::string>(path, root));
}

void Webserv::get(std::string path, FunctionType func)
{
	_get.insert(std::pair<std::string, FunctionType>(path, func));
}

void Webserv::getResponse(Request req, int fd)
{
	// DEBUG:
	std::cout << "GET Response: " << _static_folders[req.path] + req.file << std::endl;
	std::cout << "path: " << req.path << std::endl;
	std::cout << "folder: " << req.folder << std::endl;
	std::cout << "file: " << req.file << std::endl;

	if (_get.find(req.path) != _get.end())
		_get[req.path](req, fd);
	else if (file_exists(_static_folders[req.folder] + req.file))
		send_file(fd, _static_folders[req.folder] + req.file, req.mime_type);
	else if (file_exists(req.path))
		send_file(fd, req.path, req.mime_type);
	else if (req.file == "index.html" && (directoryExists(req.folder) || directoryExists(_static_folders[req.folder])))
		std::cout << listFilesInDirectory(_static_folders[req.folder]) << std::endl;
	else
		res(error_404, fd);
}

void Webserv::post(std::string path, FunctionType func)
{
	_post.insert(std::pair<std::string, FunctionType>(path, func));
}
void Webserv::postResponse(Request req, int fd)
{
	if (req.headers.find("Content-Length") == req.headers.end())
	{
		res("HTTP/1.1 411 Length Required\r\nContent-Length: 0\r\n\r\n", fd);
		return;
	}

	int content_length = atoi(req.headers["Content-Length"].c_str());

	if ((int)req.body.size() != content_length)
	{
		res("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n", fd);
		return;
	}

	if (content_length == 0)
	{
		res("HTTP/1.1 204 No Content\r\nContent-Length: 0\r\n\r\n", fd);
		return;
	}

	if (content_length > 100000) //MAX BODY SIZE
	{
		res("HTTP/1.1 413 Request Entity Too Large\r\nContent-Length: 0\r\n\r\n", fd);
		return;
	}
	if (_post.find(req.path) != _post.end()) // Pour les requetes API
		_post[req.path](req, fd);
	else
		defaultPost(req, fd);
}

void defaultPost(Request req, int fd)
{
    // Obtenir l'extension du fichier à partir du type de contenu
    std::string extension;
    if (req.headers["Content-Type"] == "application/json")
    {
        extension = "json";
    }
    else
    {
        extension = req.headers["Content-Type"].substr(req.headers["Content-Type"].find("/") + 1);
    }

	//Faut juste que je vois pour le nom du fichier...
	std::string filename = "file." + extension;
    std::ofstream file(filename.c_str(), std::ios::binary);
    file.write(req.body.c_str(), req.body.size());
    if (!file) //On verifie si le fichier a bien ete cree
    {
		std::string response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
        res(response, fd);
        return;
    }

	std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
    res(response, fd);
}

void res(std::string status, std::string headers, std::string body, int fd)
{
	std::string response = "HTTP/1.1 " + status + "\n" + headers + "\n\n" + body;
	if ((write(fd, response.c_str(), response.size())) == -1)
	{
		perror("write in res");
		exit(EXIT_FAILURE);
	}
}

void res(std::string rep, int fd)
{
	if ((write(fd, rep.c_str(), rep.size())) == -1)
	{
		perror("write in res");
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
		_static_folders = rhs._static_folders;
		_get = rhs._get;
		_post = rhs._post;
	}
	return *this;
}
//--------------Constructors------------//

Webserv::Webserv(ServerConfig &new_config) {
	_config = new_config;

	_sock_serv = Socket(_config.getPort());
	if (listen(_sock_serv.getFd(), 1) < 0)
	{
		perror("cannot listen");
		exit(1);
	}

	try {
		std::map<std::string, std::map<std::string, std::string> > locations = _config.getLocations();
		std::map<std::string, std::map<std::string, std::string> >::iterator it;
		if (locations.empty())
			throw std::runtime_error("No locations found");
		for (it = locations.begin(); it != locations.end(); it++)
		{
			use(it->first, _config.getLocationValue(it->first, "root"));
			std::cout << "Location: " << it->first << std::endl;
			std::cout << _config.getLocationValue(it->first, "root") << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
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
void	Webserv::serverLoop(std::map<int, Webserv> map_serv)
{
	struct timeval          timeout;
	std::string             request;
	Socket                  tmp_socket;
	fd_set                  read_set;
    fd_set                  write_set;
	fd_set                  tmp_write_set;
	fd_set                  tmp_read_set;
	ssize_t                 tmp_ret_value;
	int                     max_fd = 0;
    std::map<int, Request>  map_req;
	std::map<int, Socket>   sock_clients;

	FD_ZERO(&read_set);
    FD_ZERO(&write_set);
	for (std::map<int, Webserv>::iterator it = map_serv.begin(); it != map_serv.end(); it++)
	{
		FD_SET(it->first, &read_set);
		max_fd = it->first;
	}

	signal(SIGINT, sigint_handler);
	while (!flag)
	{
		tmp_read_set = read_set;
		tmp_write_set = write_set;
		timeout.tv_sec = TIMEOUT_SEC;
		timeout.tv_usec = 0;
		std::cout << "\tWaiting for something to happen.."<<std::endl;
		tmp_ret_value = select(max_fd + 1, &tmp_read_set, &tmp_write_set, NULL, &timeout);
		if (tmp_ret_value < 0)
		{
			perror("select:");
			exit(EXIT_FAILURE);
		}
		if (tmp_ret_value == 0)
        {
            closeFds(read_set, max_fd, &map_serv);
            closeFds(write_set, max_fd);
			continue;
        }
        check_hanging(read_set, write_set, tmp_read_set, tmp_write_set, sock_clients, max_fd, map_req, map_serv);
		for (int i = 0; i <= max_fd; i++)
		{
			if (FD_ISSET(i, &tmp_read_set))
			{
				if (fd_is_serv(i, map_serv, tmp_read_set))
				{
					Socket::Accept(map_serv[i]._sock_serv, tmp_socket);
					sock_clients.insert(std::pair<int, Socket>(tmp_socket.getFd(), tmp_socket));
					tmp_socket.showInfo();
					FD_SET(tmp_socket.getFd(), &read_set);
					if (tmp_socket.getFd() > max_fd)
						max_fd = tmp_socket.getFd();
                    sock_clients[i].updateLastActivity();
				}
				else
				{
					if ((request = getRequest(i, read_set)).empty())
                    {
                        sock_clients.erase(i);
						continue;
                    }

                    FD_SET(i, &write_set);
                    FD_CLR(i, &read_set);
					Request req(&(map_serv[sock_clients[i].server_fd]._static_folders), &write_set);
					parseRequest(request, req);
                    map_req[i] = req;

					sock_clients[i].showInfo();
                    sock_clients[i].updateLastActivity();
				}
			}
            else if (FD_ISSET(i, &tmp_write_set))
            {
				map_serv[sock_clients[i].server_fd].sendResponse(i, map_req[i], sock_clients[i].getIp());
                map_req.erase(i);
                sock_clients.erase(i);
            }
			if (flag)
				break;
		}
	}
	closeFds(read_set, max_fd);
}
void	Webserv::closeFds(fd_set& set, int max_fd, std::map<int, Webserv>* map_serv)
{
    if  (map_serv == NULL)
    {
        for (int i = 0; i < max_fd + 1; i++)
        {
            if (FD_ISSET(i, &set))
            {
                close(i);
                FD_CLR(i, &set);
            }
        }
    }
    else
    {
        for (int i = 0; i < max_fd + 1; i++)
        {
            if (map_serv->find(i) == map_serv->end() && FD_ISSET(i, &set))
            {
                close(i);
                FD_CLR(i, &set);
            }
        }
    }
}
std::string Webserv::getRequest(int fd, fd_set& set)
{
	int		 bytes_read = 0;
	char		buffer[BUFFER_SIZE];
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
int fd_is_serv(int fd, std::map<int, Webserv>& map_serv, fd_set& set)
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
void	sigint_handler(int signal)
{
    (void) signal;
	std::cout << "Stopping server : sigint" <<std::endl;
	flag = 1;
}

void send_file(int fd, const std::string& path, const std::string& mime_type)
{

	std::ifstream	   in(path.c_str(), std::ios::in | std::ios::binary);
	std::stringstream   buff;
	buff << in.rdbuf();
	in.close();

	std::ostringstream oss;
	oss << buff.str().size();
	
	std::string response = "HTTP/1.1 200 OK\n\
	Access-Control-Allow-Origin: *\n\
	Content-Length: ";
	response += oss.str();
	response += "\nContent-Type: ";
	response += mime_type;
	response += "\n\n";
	response += buff.str();
	res(response, fd);
	// std::string ok = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
	// write(fd, ok.c_str(), ok.size());
}

bool file_exists(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	return file.good();
}

void            check_hanging(fd_set& read_set, fd_set& write_set, fd_set& tmp_read_set,
                                fd_set& tmp_write_set, std::map<int, Socket>& sock_clients, int max_fd,
                                std::map<int, Request>& map_req, std::map<int, Webserv>& map_serv)
{
    struct timeval tmp;
    gettimeofday(&tmp, NULL);

    for (int i = 0; i < max_fd + 1; i++)
    {
        if (fd_is_serv(i, map_serv, read_set) || FD_ISSET(i, &tmp_read_set) || FD_ISSET(i, &tmp_write_set)
                || (!FD_ISSET(i, &read_set) && !FD_ISSET(i, &write_set)) 
                ||  tmp.tv_sec - sock_clients[i].getLastActivity().tv_sec < TIMEOUT_SEC)
            continue;
        if (FD_ISSET(i, &read_set))
            FD_CLR(i, &read_set);
        if (FD_ISSET(i, &write_set))
            FD_CLR(i, &write_set);
        close(i);
        sock_clients.erase(i);
        map_req.erase(i);
    }
}
