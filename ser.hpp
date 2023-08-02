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
#include "channel.hpp"

#define g_max_clients 10

class server
{
private:
    std::vector<struct pollfd> pfds;
    const char *_port;
    std::string _password;
    int sfd;

public:
    // Client clients[g_max_clients];
    Client clients;
    channel channels;
    std::map<std::string, channel>chan_map;
    std::map<int , Client> cl;
    server(const char *port, const char *password);
    int server_socket();
    int authenticateClient(std::vector<std::string> c, int listener);
    bool nickname_cmd(std::vector<std::string> &vec, int c_fd);
    bool user_cmd(std::vector<std::string>vec, int c_fd);
    bool is_identical(std::string nick, int cfd);
    bool is_channelexist(std::string name);
    void removeclients(int cfd, int cfd2);
    bool handle_recievers(std::vector<std::string> vec, int c_fd);
    bool send_messg(std::string mssg, int client_fd);
    int server_setup();
    bool is_operator(std::string chan_name, int client_fd);
    channel get_channel(std::string);
    int get_clientfd(std::string name);
    int get_port();
    void set_sfd(int sfd);
    std::string get_pass();
    bool cmd_handler(char *mssg, int listener, int client_fd);
    ~server();
};

// std::ostream &operator<<(std::ostream & o, std::map<std::string, channel> const& m) {

//     for(std::map<std::string, channel>::const_iterator it = m.begin(); it != m.end(); it++)
//     {
//         std::vector<int> ls = it->second.clients_fd;
//         for (int i = 0; i < ls.size(); i++)
//             o << ls[i] << std::endl;
//     }
//     return o;
// }

#endif