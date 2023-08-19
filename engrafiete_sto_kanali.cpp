#include"ser.hpp"


bool server::client_exist(std::string name, int client_fd)
{
    std::vector<int> fd = chan_map[name]->clients_fd;
    std::vector<int> opers = chan_map[name]->_operators_fd;
    for (size_t i = 0; i < fd.size(); i++)
    {
        if (client_fd == fd[i])
            return true;
    }
    for (size_t i = 0; i < opers.size(); i++)
    {
        if (client_fd == opers[i])
            return true;
    }
    return false;
}

bool server::clinet_invited(std::string name, int client_fd)
{
    std::vector<int> fd = chan_map[name]->invited_members;
    for (size_t i = 0; i < fd.size(); i++)
    {
        if (client_fd == fd[i])
            return true;
    }
    return false;
}

bool server::engrafiete_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    // printed_ascii only in channel name
    if (/*vec.size() > 3 ||*/ vec.size() < 2 || (vec.size() == 2 && vec[1] == "#"))
    {
        std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
        return false;
    }
    std::vector<std::string> chans = splite(vec[1], ',');
    std::vector<std::string> key;
    if (vec.size() > 2 && !vec[2].empty())
    {
        key = splite(vec[2], ',');
    }
    for (size_t i = 0; i < chans.size(); i++)
    {
        if (*chans[i].begin() != '#' && *chans[i].begin() != '&')
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 403 " + cl.at(client_fd).get_nickname();
            err += chans[i] + " :No such channel\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
        }
        else if (is_channelexist(chans[i]) == false)
        {
            // NO third param
            channels = new channel();
            if (key.empty())
            {
                if (channels->add_channel(chans[i], cl.at(client_fd), false) == true)
                    chan_map[chans[i]] = channels;
                reply(chans[i], client_fd, true);
            }
            // there is third param and key seted for the channel
            else if (!key.empty() && !key[i].empty())
            {
                if (channels->add_channel(chans[i], cl.at(client_fd), true) == true)
                {
                    chan_map[chans[i]] = channels;
                    chan_map[chans[i]]->chan_password = key[i];
                }
                reply(chans[i], client_fd, true);

            }
            // there is third param and no key to set for this channel
            else if ((!key.empty() && key[i].empty()))
                if (channels->add_channel(chans[i], cl.at(client_fd), false) == true)
                {
                    chan_map[chans[i]] = channels;
                    reply(chans[i], client_fd, true);
                }
        }
     // check security of channel
        else if (is_channelexist(chans[i]) == true && chan_map[chans[i]]->secure == false)
        {
            if (client_exist(chans[i], client_fd) == true)
                return true;
            else if (chan_map[chans[i]]->is_inviteonly)
            {
                if (clinet_invited(chans[i], client_fd) ==  true)
                {
                    if (chan_map[chans[i]]->is_limited == true)
                    {
                        if (chan_map[chans[i]]->nbr_member < chan_map[chans[i]]->user_limite)
                        {
                            chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                            chan_map[chans[i]]->nbr_member++;
                            reply(chans[i], client_fd, true);
                        }
                        else
                        {
                            std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                            err += " " + chans[i] + " :Cannot join channel (+l)\r\n";
                            const char *buff = err.c_str();
                            send(client_fd, buff, strlen(buff), 0);
                        }
                    }
                    else if (chan_map[chans[i]]->is_limited == false)
                    {
                        chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                        chan_map[chans[i]]->nbr_member++;
                        reply(chans[i], client_fd, true);
                    }
                }
                else
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 473 " + cl[client_fd].get_nickname();
                    err += " " + chans[i] + " :Cannot join channel (+i)\r\n";
                    const char *buff = err.c_str(); 
                    send(client_fd, buff, strlen(buff), 0);
                }
            }
            else if (chan_map[chans[i]]->is_inviteonly == false && chan_map[chans[i]]->is_limited)
            {
                if (chan_map[chans[i]]->nbr_member < chan_map[chans[i]]->user_limite)
                {
                    chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                    chan_map[chans[i]]->nbr_member++;
                    reply(chans[i], client_fd, true);
                }
                else
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                    err += " " + chans[i] + " :Cannot join channel (+l)\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                }
            }
            else if (chan_map[chans[i]]->is_inviteonly == false && chan_map[chans[i]]->is_limited == false)
            {
                chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                chan_map[chans[i]]->nbr_member++;
                reply(chans[i], client_fd, true);
            }
        }
        
        else if (is_channelexist(chans[i]) == true && chan_map[chans[i]]->secure == true)
        {
            if (client_exist(chans[i], client_fd) == true)
                return true;
            if (!key.empty() && !key[i].empty())
            {
                if (chan_map[chans[i]]->is_inviteonly)
                {
                    if (clinet_invited(chans[i], client_fd))
                    {
                        if (key[i] == chan_map[chans[i]]->chan_password)
                        {
                            if ((chan_map[chans[i]]->user_limite > chan_map[chans[i]]->nbr_member) || (chan_map[chans[i]]->user_limite == 0))
                            {
                                chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                                chan_map[chans[i]]->nbr_member++;
                                reply(chans[i], client_fd, true);
                            }
                            else
                            {
                                std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                                err += " " + chans[i] + " :Cannot join channel (+l)\r\n";
                                const char *buff = err.c_str();
                                send(client_fd, buff, strlen(buff), 0);
                            }
                        }
                        else if (key[i] != chan_map[chans[i]]->chan_password)
                        {
                            std::string err = ":" + cl[client_fd].get_host() + " 475 " + cl[client_fd].get_nickname();
                            err += " " + chans[i] + " :Cannot join channel (+k)\r\n";
                            const char *buff = err.c_str();
                            send(client_fd, buff, strlen(buff), 0);
                        }
                    }
                    else
                    {
                        std::string err = ":" + cl[client_fd].get_host() + " 473 " + cl[client_fd].get_nickname();
                        err += " " + chans[i] + " :Cannot join channel (+i)\r\n";
                        const char *buff = err.c_str();
                        send(client_fd, buff, strlen(buff), 0);
                    }
                }
                else if (chan_map[chans[i]]->is_inviteonly == false && chan_map[chans[i]]->is_limited == true)
                {
                    if (key[i] == chan_map[chans[i]]->chan_password)
                    {
                        if ((chan_map[chans[i]]->user_limite > chan_map[chans[i]]->nbr_member) || (chan_map[chans[i]]->user_limite == 0))
                        {
                            chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                            chan_map[chans[i]]->nbr_member++;
                            reply(chans[i], client_fd, true);
                        }
                        else
                        {
                            std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                            err += " " + chans[i] + " :Cannot join channel (+l)\r\n";
                            const char *buff = err.c_str();
                            send(client_fd, buff, strlen(buff), 0);
                        }
                    }
                    else
                    {
                        std::string err = ":" + cl[client_fd].get_host() + " 475 " + cl[client_fd].get_nickname();
                        err += " " + chans[i] + " :Cannot join channel (+k)\r\n";
                        const char *buff = err.c_str();
                        send(client_fd, buff, strlen(buff), 0);
                    }                
                }
                else
                {
                    if (key[i] == chan_map[chans[i]]->chan_password)
                    {
                        chan_map[chans[i]]->add_member(cl.at(client_fd), chans[i]);
                        chan_map[chans[i]]->nbr_member += 1;
                        reply(chans[i], client_fd, true);
                    }
                    else
                    {
                        std::string err = ":" + cl[client_fd].get_host() + " 475 " + cl[client_fd].get_nickname();
                        err += " " + chans[i] + " :Cannot join channel (+k)\r\n";
                        const char *buff = err.c_str();
                        send(client_fd, buff, strlen(buff), 0);
                    }
                }
            }
            else
            {
                std::string err = ":" + cl[client_fd].get_host() + " 475 " + cl[client_fd].get_nickname();
                err += " " + chans[i] + " :Cannot join channel (+k)\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
        }
    }
    return true;
}

//:los!~p@5c8c-aff4-7127-3c3-1c20.230.197 JOIN :#there

bool server::reply(std::string name, int cfd, bool flag)
{
    // lr!~k@5c8c-aff4-7127-3c3-1c20.230.197 JOIN :#there
    // :punch.wa.us.dal.net 353 lr = #there :lr @lop 
    // :punch.wa.us.dal.net 366 lr #there :End of /NAMES list
    std::vector<int> fds = chan_map[name]->get_chan_member();
    std::string chan_members;
    if (fds.size() >= 1)
    {
        for (size_t i = 0; i < fds.size(); i++)
        {
            std::string tmp;
            tmp = cl[fds[i]].get_nickname();
            chan_members += tmp;
            chan_members += " ";
            std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
            notif += " JOIN :" + name + "\r\n";
            const char *rpl = notif.c_str();
            if (fds[i] != cfd)
                send(fds[i], rpl, strlen(rpl), 0);
        }
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (size_t j =0; j < opers.size(); j++)
    {
        std::string tmpl;
        tmpl = cl[opers[j]].get_nickname();
        chan_opers += tmpl;
        std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
        notif += " JOIN :" + name + "\r\n";
        const char *rpl = notif.c_str();
        if (opers[j] != cfd)
            send(opers[j], rpl, strlen(rpl), 0);
        if (j != (opers.size() - 1))
            chan_opers += " ";
    }
    
    if (flag == true)
    {
        std::string rpl = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip() + " JOIN :"+ name + "\r\n";
        rpl += ":" + cl[cfd].get_host() + " 353 " + cl[cfd].get_nickname() + " = " + name + " :" + chan_members + "@" + chan_opers + "\r\n"; 
        rpl += ":" + cl[cfd].get_host() + " 366 " + cl[cfd].get_nickname() + " " + name + " :End of /NAMES list.";
        rpl += "\r\n";
        const char *buff = rpl.c_str();
        send(cfd, buff, strlen(buff), 0);
    }
    return true;
}
