#include "Socket.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

//--------------Functions----------------//
uint32_t    convertIPtoBinary(const std::string& ipAddress);
void        printBinary(uint32_t num);
std::string addrToString(uint32_t ip);

std::string Socket::getIp() const
{
    return addrToString(ntohl(this->sockaddr.sin_addr.s_addr));
}
void  Socket::updateLastActivity()
{
    gettimeofday(&(this->_last_activity), NULL);
}

struct timeval  Socket::getLastActivity() const
{
    return this->_last_activity;
}

int Socket::bindSocket(int& sockfd, struct sockaddr_in const& address)
{
    return bind(sockfd, reinterpret_cast<const struct sockaddr*>(&address), sizeof(address));
}

void Socket::showInfo() const
{
    std::cout << "Socket fd:" << socket_fd << ", IP:" <<
        addrToString(ntohl(sockaddr.sin_addr.s_addr)) << ", PORT:" << ntohs(sockaddr.sin_port) << std::endl;
}

int Socket::getFd(void) const
{
    return socket_fd;
}
//--------------Operators----------------//
Socket& Socket::operator=(Socket const&  rhs)
{
    if (this != &rhs)
    {
        this->sockaddr = rhs.sockaddr;
        this->sockaddr_size = rhs.sockaddr_size;
        this->socket_fd = rhs.socket_fd;
    }
    return *this;
}
//--------------Constructors------------//
Socket::Socket(Socket const &src):  socket_fd(src.socket_fd),
                                    server_fd(src.server_fd),
                                    sockaddr(src.sockaddr),
                                    sockaddr_size(src.sockaddr_size)
{
}

Socket::Socket(uint16_t port) : socket_fd(socket(AF_INET, SOCK_STREAM, 0)),
                                server_fd(0),
                                sockaddr()
{
    if (socket_fd < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't initialize");
        return;
    }
    if (fcntl(socket_fd, F_SETFL, (fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK) < 0))
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't set to non block");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't set reuse flag");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    if (bindSocket(socket_fd, sockaddr) < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't bind");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    sockaddr_size = sizeof(sockaddr);
    std::cout << "Successfully created and bound socket.\n";
    this->showInfo();
}
Socket::Socket(std::string ip, uint16_t port) : socket_fd(socket(AF_INET, SOCK_STREAM, 0)),
                                                server_fd(0),
                                                sockaddr()
{
    if (socket_fd < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't initialize");
        return;
    }
    if (fcntl(socket_fd, F_SETFL, (fcntl(socket_fd, F_GETFL, 0) | O_NONBLOCK) < 0))
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't set to non block");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't set reuse flag");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = htonl(convertIPtoBinary(ip));
    if (bindSocket(socket_fd, sockaddr) < 0)
    {
        std::cerr << "socket:" << port << " ";
        perror("Couldn't bind");
        close(socket_fd);
        socket_fd = -1;
        return;
    }
    sockaddr_size = sizeof(sockaddr);
    std::cout << "Successfully created and bound socket.\n";
    this->showInfo();
}
Socket::Socket(void) :  socket_fd(-1),
                        server_fd(0),
                        sockaddr(),
                        sockaddr_size(sizeof(sockaddr))
{
}

Socket::~Socket(void)
{
}
//--------------Non-Member--------------//
void    Socket::Accept(Socket& listening_socket, Socket& calling_socket)
{
    calling_socket.socket_fd = accept(listening_socket.getFd(),
                        reinterpret_cast<struct sockaddr*>(&calling_socket.sockaddr),
                        &calling_socket.sockaddr_size);
    if (calling_socket.getFd() < 0 || calling_socket.sockaddr.sin_family != AF_INET)
    {
        perror("couldn't accept connection");
        exit(1);
    }
    if (fcntl(calling_socket.getFd(), F_SETFL, (fcntl(calling_socket.getFd(), F_GETFL, 0) | O_NONBLOCK) < 0))
    {
        perror("Couldn't set to non block");
        exit(1);
    }
    calling_socket.server_fd = listening_socket.getFd(); 
    std::cout << "ACCEPTED: "; calling_socket.showInfo();
}
std::string addrToString(uint32_t ip)
{
    std::stringstream str;
    str << (ip >> 24) << "." << ((ip & 0x00FF0000) >> 16) << "." << 
 ((ip & 0x0000FF00) >> 8) << "." << (ip & 0x000000FF); 
    std::string ret;
    str >> ret;
    return ret;
}
uint32_t    convertIPtoBinary(const std::string& ipAddress)
{
    uint32_t    result = 0;
    uint32_t    octet = 0;
    int         shift = 24;
    
    for (size_t i = 0; i < ipAddress.size(); ++i)
    {
        if (ipAddress[i] == '.')
        {
            result |= (octet << shift);
            octet = 0;
            shift -= 8;
        }
        else if (isdigit(ipAddress[i]))
            octet = octet * 10 + (ipAddress[i] - '0');
        else
            return 0;
    }
    
    result |= (octet << shift);
    return result;
}
void printBinary(uint32_t num)
{
    for (int i = sizeof(int) * 8 - 1; i >= 0; --i)
        std::cout << ((num >> i) & 1);
    std::cout << std::endl;
}
