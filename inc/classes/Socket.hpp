#ifndef		SOCKET_HPP
# define	SOCKET_HPP

#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

class	Socket
{

public:
    struct timeval  getLastActivity() const;
    void            updateLastActivity();
    void            showInfo() const;
    int             bindSocket(int& sockfd, const struct sockaddr_in& address);
    int             getFd(void) const;
    static void     Accept(Socket& listening_socket, Socket& calling_socket);

    Socket(std::string ip, uint16_t port);
    Socket(uint16_t port);
	Socket(Socket const & src);
	Socket&	operator=(Socket const& rhs);
	Socket(void);
	~Socket(void);

    int                 socket_fd;
    int                 server_fd;
    struct sockaddr_in  sockaddr;
    socklen_t           sockaddr_size;

private:
    struct timeval  _last_activity;

};

#endif
