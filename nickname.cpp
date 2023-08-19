#include "ser.hpp"

void server::removeclients(int cfd, int cfd2)
{
    //handle nickcollision error mssg
    close(cfd);
    close(cfd2);
    std::vector<struct pollfd>::iterator pit;
    for (pit= pfds.begin(); pit!= pfds.end(); ++pit){
        if(pit->fd == cfd2)
            break;
    }
    pfds.erase(pit);
    for (pit = pfds.begin(); pit!= pfds.end(); ++pit){
        if(pit->fd == cfd)
            break;
    }
    pfds.erase(pit);
}

bool server::is_identical(std::string nick, int c_fd){
    std::map<int, Client>::iterator m_it;
    std::string nickname;

    for(m_it = cl.begin(); m_it != cl.end(); ++m_it){
        if (m_it->first != c_fd){
            nickname = m_it->second.get_nickname();
            if (!nickname.empty() && (nickname == nick)){
                return true;
            }
        }
    }
    return false;
}

bool server::valid_nick(int c_fd, std::string str)
{
    for(size_t i= 0; i <= str.length(); i++)
    {
        if (strchr("0123456789-", str[0]))
        {
            // :punch.wa.us.dal.net 432 los 135 :Erroneous Nickname
            std::string errnouse = ":" + cl.at(c_fd).get_host() + " 432 " + cl.at(c_fd).get_nickname() + str;
            errnouse += " :Erroneous Nickname\r\n";
            const char *buff = errnouse.c_str();
            send(c_fd, buff, strlen(buff), 0);
            return false;
        }
        if (!strchr("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz|^_-{}[]\\", str[i]))
        {
            std::string errnouse = ":" + cl.at(c_fd).get_host() + " 432 " + cl.at(c_fd).get_nickname() + str;
            errnouse += " :Erroneous Nickname\r\n";
            const char *buff = errnouse.c_str();
            send(c_fd, buff, strlen(buff), 0);
            return false;
        }
    }
    return true;
}


bool server::nickname_cmd(std::vector<std::string> &vec, int c_fd)
{
    //:los!~d@5c8c-aff4-7127-3c3-1c20.230.197 NICK :lsp
    // allowed chars : ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz`|^_-{}[]\'
    std::string nick;
    if (vec.size() < 2)
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 431 " + cl.at(c_fd).get_nickname();
        err += " :No nickname given\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
        return false;
    }
    if (valid_nick(c_fd, vec[1])== false)
        return false;
    else if (cl[c_fd].get_nickname() != " " && is_identical(vec[1], c_fd) == false)
    {
        if (*vec[1].begin() == ':')
            nick = vec[1].substr(1);
        else
            nick = vec[1];
        std::string err = ":" + cl.at(c_fd).get_nickname() + "!~" + cl.at(c_fd).get_username() + "@" + cl.at(c_fd).get_clientip();
        err += " NICK :" + vec[1] + "\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
        std::map<int,Client>::iterator it = cl.find(c_fd);
        it->second.set_nickname(trim(nick, " \t\n\r"));
    }
    else if (is_identical(vec[1], c_fd) == false)
    {
        if (*vec[1].begin() == ':')
            nick = vec[1].substr(1);
        else
            nick = vec[1];
        std::map<int,Client>::iterator it = cl.find(c_fd);
        it->second.set_nickname(trim(nick, " \t\n\r"));
        it->second.is_registred += 1;
        cl.at(c_fd).g_msg = 3;
        cl.at(c_fd).welcome++;
    }
    else
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 433 " + cl.at(c_fd).get_nickname();
        err += " :Nickname is already in use\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
    }
    return true;
}
