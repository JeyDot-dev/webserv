#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <csignal>
#include <iostream>
#include <sstream>
#include <Socket.hpp>
#include <cstdio>
#include <cstdlib>
#include <sys/select.h>
#include <unistd.h>
#include <map>
#define BUFFER_SIZE 1024
#define TIMEOUT_SEC 8
void    close_fds(fd_set set, int max_fds)
{
    for (int i = 0; i < max_fds; i++)
    {
        if (FD_ISSET(i, &set))
            close(i);
    }
}
int main(int ac, char **av)
{
    (void) ac;
    Socket                  a;
    std::map<int, Socket>   socket_list;
    Socket                  server(atoi(av[1]));
    fd_set                  usable, original;
    char                    buffer[BUFFER_SIZE] = {0};
    std::stringstream       read_client;
    ssize_t                 bytes_read;
    int                     max_fd_set_size;
    struct timeval          timeout;
    int                     select_ret;
    std::string             response = "\
HTTP/1.1 200 OK\n\
Content-Type: text/plain\n\
Content-Length: 12\n\n\
Hello world!";

    if (listen(server.getFd(), 1) < 0)
    {
        perror("cannot listen");
        exit(1);
    }
    max_fd_set_size = server.getFd();
    FD_ZERO(&original);
    FD_SET(server.getFd(), &original);

    while (true)
    {
        usable = original;
        timeout.tv_sec = TIMEOUT_SEC;
        timeout.tv_usec = 0;
        std::cout << "\tWaiting for something to happen.."<<std::endl;
        select_ret = select(max_fd_set_size + 1, &usable, NULL, NULL, &timeout);
        if (select_ret < 0)
        {
            perror("select:");
            exit(EXIT_FAILURE);
        }
        if (select_ret == 0)
            break;
        for (int i = 0; i <= max_fd_set_size; i++)
        {
            if (FD_ISSET(i, &usable))
            {
                if (i == server.getFd())
                {
                    Socket::Accept(server, a);
                    socket_list.insert(std::pair<int, Socket>(a.getFd(), a));
                    a.showInfo();
                    FD_SET(a.getFd(), &original);
                    if (a.getFd() > max_fd_set_size)
                        max_fd_set_size = a.getFd();
                }
                else
                {
                    if ((bytes_read = read(i, buffer, BUFFER_SIZE)) > 0)
                        read_client.write(buffer, bytes_read);
                    else if (bytes_read == -1)
                    {
                        std::cout << "\t\tDAWJOPOADJW" << std::endl;
                    }
                    else
                    {
                        close(i);
                        FD_CLR(i, &original);
                        break;
                    }
                    std::string http_request = read_client.str();
                    read_client.str("");
                    //std::cout << http_request << std::endl;
                    write(i, response.c_str(), response.size());
                    socket_list[i].showInfo();
                }
            }
        }
    }
    close_fds(original, max_fd_set_size);
    return 0;
}
