#include"client.hpp"

Client::Client(bool is_connected, std::string nick, std::string user) : is_authentifcated(is_connected)\
,nickname(nick), username(user)
{
}


void Client::set_authent(bool isauthent){is_authentifcated = isauthent;}

bool Client::get_authent() const {return is_authentifcated;}
void Client::set_nickname(std::string nick){
    nickname = nick;
}
Client::~Client()
{
}
