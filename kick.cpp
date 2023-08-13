#include"ser.hpp"


void server::kick_rply(std::string name, int oper, int cfd)
{
    //ver = oper / lop = member
    //:ver!~ver@5c8c-aff4-7127-3c3-1c20.230.197.ip KICK #there lop :ver
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip KICK " + name + cl[cfd].get_nickname() + " :" + cl[oper].get_nickname() + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    for (int j = 0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip KICK " + name + cl[cfd].get_nickname() + " :" + cl[oper].get_nickname() + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }
}

bool server::kick_memeber(std::vector<std::string> vec, int client_fd)
{
    if (is_channelexist(vec[1]) == true)
    {
        if (is_operator(vec[1], client_fd) ==  true)
        {
            int cfd = get_clientfd(vec[2]);
            if (cfd < 0)
            {
                std::string err = ":" + cl[client_fd].get_host() + " 401 " + cl[client_fd].get_nickname() + vec[1] + " :No such nick/channel\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
            else if (chan_map[vec[1]]->is_member(cfd, vec[1]) == false && \
            chan_map[vec[1]]->is_operator(vec[1], cfd) == false)
            {
                // :punch.wa.us.dal.net 441 losp posx #top :They aren't on that channel
                std::string err = ":" + cl[client_fd].get_host() + " 441 " + cl[client_fd].get_nickname() + vec[3] + vec[1] \
                + " :They aren't on that channel\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
                return false;
            }
            else if (chan_map[vec[1]]->is_operator(vec[1], cfd) == true)
            {
                if (chan_map[vec[1]]->_operators_fd.size() == 1)
                {
                    std::map<std::string, channel*>::iterator it;
                    for (it = chan_map.begin(); it != chan_map.end(); ++it)
                    {
                        if (it->first == vec[1])
                        {
                            delete it->second;
                            chan_map.erase(it);
                            break;
                        }
                    }
                    kick_rply(vec[1], client_fd, cfd);
                }
                else if (chan_map[vec[1]]->_operators_fd.size() > 1)
                {
                    std::vector<int>::iterator vit;
                    for(vit = chan_map[vec[1]]->_operators_fd.begin(); vit != chan_map[vec[1]]->_operators_fd.end(); ++vit)
                    {
                        if (*vit == cfd)
                        {
                            chan_map[vec[1]]->_operators_fd.erase(vit);
                            kick_rply(vec[1], client_fd, cfd);
                        }
                    }
                }
            }
            else if (chan_map[vec[1]]->is_member(cfd, vec[1]) == true)
            {
                chan_map[vec[1]]->remove_member(cl.at(cfd), vec[1]);
                kick_rply(vec[1], client_fd, cfd);
            }
        }
        else
        {
            std::string err = ":" + cl[client_fd].get_host() + " 482 " + cl[client_fd].get_nickname();
            err += " :You're not channel operator\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
        }
    }
    else
    {
        std::string err = ":" + cl[client_fd].get_host() + " 403 " + cl[client_fd].get_nickname();
        err += " :No such channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    return true;
}