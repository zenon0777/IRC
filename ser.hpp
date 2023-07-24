#ifndef SER_HPP

#define SER_HPP
#include<iostream>
#include<sys/socket.h>
#include<string>
#include<sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include<poll.h>
#include<fcntl.h>
#include<fstream>
#include<unistd.h>
#include<map>
#include"client.hpp"

class server
{
private:
    std::vector<struct pollfd> pfds;
    const char *_port;
    const char *_password;
    int sfd;
public:
    std::vector<struct pollfd> pfds;
    std::map<int i, client *client> clients;
    server(const char *port, const char *password);
    int server_socket();
    int server_setup();
    int get_port();
    void set_sfd(int sfd);
    std::string get_pass();
    bool cmd_handler(char *mssg, int listener);
    ~server();
};



#endif