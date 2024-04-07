#include "Cgi.hpp"
#include <algorithm>

//--------------Functions----------------//
std::vector<std::string>    Cgi::getEnv() const
{
    return _env_vars;
}
std::string                 Cgi::getPath() const
{
    size_t start = _env_vars[PATH_INFO].find_first_of("=") + 1;
    if (start >= _env_vars[PATH_INFO].size())
        return std::string();
    std::string path = _env_vars[PATH_INFO].substr(start);
    return path;
}
std::string                 Cgi::getScriptPath() const
{
    size_t start = _env_vars[SCRIPT_FILENAME].find_first_of("=") + 2;
    if (start >= _env_vars[SCRIPT_FILENAME].size())
        return std::string();
    std::string path = _env_vars[SCRIPT_FILENAME].substr(start);
    return path;
}
//--------------Operators----------------//
Cgi&	Cgi::operator=(Cgi const&  rhs)
{
	if (this != &rhs)
	{
        _env_vars = rhs._env_vars;
	}
	return *this;
}
//--------------Constructors------------//
Cgi::Cgi(Request req, std::string client_ip, std::string host_ip)
{
    _env_vars.push_back("CONTENT_TYPE=" + req.mime_type);
    _env_vars.push_back("CONTENT_LENGTH=" + req.headers["Content-Length"]);
    _env_vars.push_back("HTTP_COOKIE=");
    _env_vars.push_back("HTTP_USER_AGENT=" + req.headers["User-Agent"]);
    if (req.path.size() > 3 && req.path.substr(req.path.size() - 3) == ".py")
    _env_vars.push_back("PATH_INFO=/usr/bin/python3");
    else if (req.path.size() > 4 && req.path.substr(req.path.size() - 4) == ".php")
    _env_vars.push_back("PATH_INFO=/usr/bin/php");
    else
    _env_vars.push_back("PATH_INFO=");
    //TODO QUERY STRING 
    _env_vars.push_back("QUERY_STRING=" + req.query);
    _env_vars.push_back("REMOTE_ADDR=" + client_ip);
    _env_vars.push_back("REMOTE_HOST=" + client_ip);
    _env_vars.push_back("REQUEST_METHOD=" + req.method);
    _env_vars.push_back("SCRIPT_FILENAME=" + req.path);
    _env_vars.push_back("SCRIPT_NAME=" + req.file);
    _env_vars.push_back("SERVER_NAME=" + host_ip);
    _env_vars.push_back("SERVER_SOFTWARE=Webserv2077");
}
Cgi::Cgi(Cgi const &src) : _env_vars(src._env_vars)
{
	*this = src;
	std::cout << "Copy constructor called" << std::endl;
	return ;
}

Cgi::Cgi(void) : _env_vars()
{
	return ;
}

Cgi::~Cgi(void)
{
	return ;
}
//--------------Non-Member--------------//
