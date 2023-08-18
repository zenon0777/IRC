#include"ser.hpp"

std::string trim(const std::string &str, std::string set)
{
    size_t first = str.find_first_not_of(set);
    if (first == std::string::npos)
    {
        return "";}
    size_t last = str.find_last_not_of(set);
    return str.substr(first, last - first + 1);
}

std::vector<std::string> server::splite(std::string str, char delim)
{
    std::istringstream ss(str);
    std::string token;
    std::vector<std::string> vec;
    while (std::getline(ss, token, delim))
    {
        token = trim(token, " \t\n\r");
        vec.push_back(token);
    }
    return vec;
}


int server::get_clientfd(std::string name)
{
    std::map<int, Client>::iterator it;
    for (it = cl.begin(); it!= cl.end();++it){
        if (it->second.get_nickname().compare(name) == 0)
            return it->first;
    }
    return -1;
}

bool server::is_channelexist(std::string name)
{
    std::map<std::string, channel *>::iterator mit;
    for (mit = chan_map.begin(); mit != chan_map.end(); ++mit){
        if (mit->first == name)
            return true;
    }
    return false;
}

void server::set_sfd(int fd)
{
    sfd = fd;
}