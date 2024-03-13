#include <iostream>
#include <sstream>
#include <Socket.hpp>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#define BUFFER_SIZE 1024
int main(void)
{
    Socket              a;
    Socket              b(8081);
    char                buffer[BUFFER_SIZE] = {0};
    std::stringstream   read_client;
    std::string         http_request;
    ssize_t             bytes_read;
    std::string         response = "\
HTTP/1.1 200 OK\n\
Content-Type: text/plain\n\
Content-Length: 12\n\n\
Hello world!";

    if (listen(b.getFd(), 1) < 0)
    {
        perror("cannot listen");
        exit(1);
    }

    std::cout << "\tWaiting for connection.."<<std::endl;
    Socket::Accept(b, a);
    if ((bytes_read = read(a.getFd(), buffer, BUFFER_SIZE)) > 0)
        read_client.write(buffer, bytes_read);
    http_request = read_client.str();
    write(a.getFd(), response.c_str(), response.size());

    close(a.getFd());
    close(b.getFd());
    std::cout << http_request << std::endl;
}
