#include"ser.hpp"

int server::authenticateClient(std::vector<std::string> c, int c_fd)
{
    if (c.size() < 2)
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 461 " + cl.at(c_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
        return false;
    }
    else 
    if (cl.at(c_fd).get_authent() == true)
        return 1;
    else if (c[1] == _password)
    {
        cl.at(c_fd).set_authent(true);
        return 0;
    }
    else
    {
        cl.at(c_fd).set_authent(false);
        return 2;
    }
    return 1;
}


bool server::is_operator(std::string chan_name, int client_fd)
{
    std::vector<int>::iterator it;
    std::map<std::string, channel *>::iterator mit;
    for (mit = chan_map.begin(); mit!= chan_map.end(); ++mit){
        if (mit->first == chan_name){
            for(it=mit->second->_operators_fd.begin(); it!= mit->second->_operators_fd.end(); ++it){
                if (*it == client_fd)
                    return true;
            }
        }
    }
    return false;
}