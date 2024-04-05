#ifndef		CGI_HPP
# define	CGI_HPP
#include <iostream>
#include <map>

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
	Cgi(Cgi const & src);
	Cgi&	operator=(Cgi const& rhs);

	Cgi(void);
	~Cgi(void);

private:
    std::map<std::string, std::string>  env_vars;

};

#endif
