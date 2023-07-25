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
    char *mssg;
public:
    Client(){}
    void set_authent(bool isauthent);
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