#include"client.hpp"


Client::Client()
{
    this->is_authentifcated = false;
    this->is_registred = 0;
}

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
    this->is_registred = obj.is_registred;
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

void Client::set_clientip(const char *remoteip){
    this->client_ip = remoteip;
}

std::string Client::get_host() const{
        return this->_host;
}

std::string Client::get_clientip() const{
    return this->client_ip;
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
