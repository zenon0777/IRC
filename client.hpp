#ifndef CLIENT_HPP
#define CLIENT_HPP
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

class Client
{
private:
    std::string nickname;
    std::string username;
    std::string real_name;
    bool        is_authentifcated;
    int         c_fd;
    std::string _host;
    std::string client_ip;
    char *mssg;
public:
    Client();
    Client(const Client &obj);
    // channel channels;
    void set_authent(bool isauthent);
    void set_host(const char *host);
    std::string get_host() const;
    void set_clientip(const char *remoteip);
    std::string get_clientip() const;
    void set_nickname(std::string nick);
    void set_username(std::string user, std::string r_name);
    std::string get_username() const;
    std::string get_realname() const;
    void set_cfd(int cfd);
    int get_clientfd() const;
    std::string get_nickname() const;
    bool get_authent() const;
    bool client_reg(std::string cmd, std::string parm);
    Client(bool is_connected, std::string nick, std::string user);
    ~Client();
};

#endif