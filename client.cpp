#include"client.hpp"

Client::Client(bool is_connected, std::string nick, std::string user) : is_authentifcated(is_connected)\
,nickname(nick), username(user)
{
}

int Client::get_clientfd() const
{
    return this->c_fd;
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
