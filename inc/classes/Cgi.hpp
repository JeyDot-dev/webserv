#ifndef		CGI_HPP
# define	CGI_HPP
#include <iostream>
#include <map>
#include "Webserv.hpp"

enum cgi_env
{
    CONTENT_TYPE,
    CONTENT_LENGTH,
    HTTP_COOKIE,
    HTTP_USER_AGENT,
    PATH_INFO,
    QUERY_STRING,
    REMOTE_ADDR,
    REMOTE_HOST,
    REQUEST_METHOD,
    SCRIPT_FILENAME,
    SCRIPT_NAME,
    SERVER_NAME,
    SERVER_SOFTWARE
};
class	Cgi
{

public:
    std::vector<std::string>    getEnv() const;
    std::string                 getPath() const;
    Cgi(Request req, std::string client_ip, std::string host_ip);
	Cgi(Cgi const & src);
	Cgi&	operator=(Cgi const& rhs);

	Cgi(void);
	~Cgi(void);

private:
    std::vector<std::string>    _env_vars;

};

#endif
