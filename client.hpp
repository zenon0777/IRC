#ifndef CLIENT_HPP
#define CLIENT_HPP
#include"ser.hpp"

class client
{
private:
    std::string nickname;
    std::string username;
    char *mssg;
public:
    bool client_reg(std::string cmd, std::string parm);
    client();
    ~client();
};

#endif