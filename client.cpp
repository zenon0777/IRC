#include"client.hpp"

Client::Client(bool is_connected, std::string nick, std::string user) : is_authentifcated(is_connected)\
,nickname(nick), username(user)
{
}
Client::Client(){this->is_authentifcated = false;}
int Client::get_clientfd() const
{
    return this->c_fd;
}

Client::Client(const Client &obj){
    this->c_fd = obj.c_fd;
    this->is_authentifcated = obj.is_authentifcated;
    this->nickname = obj.nickname;
    this->real_name = obj.real_name;
    this->username = obj.username;
}

void Client::set_username(std::string user, std::string r_name){
    this->username = user;
    this->real_name = r_name;
}

std::string Client::get_username() const{
    return this->username;
}

std::string Client::get_realname() const{
    return this->real_name;
}

void Client::set_clientip(char *remoteip){
    this->client_ip = remoteip;
}

std::string Client::get_host() const{
        return this->_host;
}

std::string Client::get_clientip() const{
    sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    std::cout << this->c_fd << std::endl;
    if (getpeername(this->c_fd, (sockaddr*)&addr, &addrLen) == -1) {
        std::cerr << "Error getting peer name: " << strerror(errno) << std::endl;
        return "";
    }

    char ipStr[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(addr.sin_addr), ipStr, INET_ADDRSTRLEN) == 0) {
        std::cerr << "Error converting IP address: " << strerror(errno) << std::endl;
        return "";
    }
    std::string ip(ipStr);
    return ip;
}

void Client::set_cfd(int cfd){
    this->c_fd = cfd;
}

void Client::set_authent(bool isauthent){this->is_authentifcated = isauthent;}

bool Client::get_authent() const {return this->is_authentifcated;}
void Client::set_nickname(std::string nick){
    this->nickname = nick;
}

void Client::set_host(const char *host){
    std::string add(host);
    this->_host = add;
}

std::string Client::get_nickname() const{
    if (this->nickname.empty())
        return " ";
    return this->nickname;}

Client::~Client()
{
}
