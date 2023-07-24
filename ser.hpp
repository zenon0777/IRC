#ifndef SER_HPP

#define SER_HPP
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <strstream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include "client.hpp"

#define g_max_clients 10

class server
{
private:
    std::vector<struct pollfd> pfds;
    const char *_port;
    std::string _password;
    int sfd;

public:
    Client clients[g_max_clients];
    // std::map<int , Client> clients;
    server(const char *port, const char *password);
    int server_socket();
    int authenticateClient(std::vector<std::string> c, int listener);
    int server_setup();
    int get_port();
    void set_sfd(int sfd);
    std::string get_pass();
    bool cmd_handler(char *mssg, int listener, int client_fd);
    ~server();
};

#endif