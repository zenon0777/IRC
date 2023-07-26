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

void Client::set_cfd(int cfd){
    this->c_fd = cfd;
}

void Client::set_authent(bool isauthent){this->is_authentifcated = isauthent;}

bool Client::get_authent() const {return this->is_authentifcated;}
void Client::set_nickname(std::string nick){
    this->nickname = nick;
}

std::string Client::get_nickname() const{return nickname;}

Client::~Client()
{
}
