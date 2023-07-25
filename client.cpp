#include"client.hpp"

Client::Client(bool is_connected, std::string nick, std::string user) : is_authentifcated(is_connected)\
,nickname(nick), username(user)
{
}

int Client::get_clientfd() const
{
    return this->c_fd;
}

void Client::set_cfd(int cfd){
    this->c_fd = cfd;
}

void Client::set_authent(bool isauthent){this->is_authentifcated = isauthent;}

bool Client::get_authent() const {return this->is_authentifcated;}
void Client::set_nickname(std::string nick){
    this->nickname = nick;
}

std::string Client::get_nickname() const{return this->nickname;}

Client::~Client()
{
}
