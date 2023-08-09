#include"ser.hpp"


bool server::engrafiete_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    if (vec.size() > 3 || vec.size() < 2 || (vec.size() == 2 && vec[1] == "#"))
    {
        std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
        return false;
    }        
    // check if user is already in the channel or not
    // list the channel member nickname to new member recently joined
    // send channel topic 
    // list to the operator all commands he can execute
    std::vector<std::string> chans = splite(vec[1], ',');
    std::vector<std::string> key;
    if (vec.size() > 2 && !vec[2].empty())
    {
        key = splite(vec[2], ',');
    }
    for (int i = 0; i < chans.size(); i++)
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
            if (key.empty())
            {
                if (channels.add_channel(chans[i], cl.at(client_fd), false) == true)
                    chan_map[chans[i]] = channels;
            }
            // there is third param and key seted for the channel
            else if (!key.empty() && !key[i].empty())
            {
                if (channels.add_channel(chans[i], cl.at(client_fd), true) == true)
                    chan_map[chans[i]] = channels;
                chan_map[chans[i]].secure = true;
                chan_map[chans[i]].chan_password = key[i];
            }
            // there is third param and no key to set for this channel
            else if ((!key.empty() && key[i].empty()) || key.empty())
                if (channels.add_channel(chans[i], cl.at(client_fd), false) == true)
                    chan_map[chans[i]] = channels;
        }
     // check security of channel
        else if (is_channelexist(chans[i]) == true && chan_map[chans[i]].secure == false)
        {
            if (chan_map[chans[i]].is_inviteonly)
            {
                std::string err = ":" + cl[client_fd].get_host() + " 473 " + cl[client_fd].get_nickname();
                err += " " + chans[i] + " :Cannot join channel (+i)\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
            else if (chan_map[chans[i]].is_inviteonly == false && chan_map[chans[i]].is_limited)
            {
                if (chan_map[chans[i]].nbr_member < chan_map[chans[i]].user_limite)
                {
                    chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                    chan_map[chans[i]].nbr_member++;
                }
                else
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                    err += " " + chans[i] + " :Cannot join channel (+l)\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                }
            }
            else if (chan_map[chans[i]].is_inviteonly == false && chan_map[chans[i]].is_limited == false)
            {
                chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                chan_map[chans[i]].nbr_member++;
            }
        }
        
        else if (is_channelexist(chans[i]) == true && chan_map[chans[i]].secure == true)
        {
            if (!key.empty() && !key[i].empty())
            {
                if (chan_map[chans[i]].is_inviteonly)
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 473 " + cl[client_fd].get_nickname();
                    err += " " + chans[i] + " :Cannot join channel (+i)\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                }
                else if (chan_map[chans[i]].is_inviteonly == false && chan_map[chans[i]].is_limited == true)
                {
                    if (key[i] == chan_map[chans[i]].chan_password)
                    {
                        if ((chan_map[chans[i]].user_limite > chan_map[chans[i]].nbr_member) || (chan_map[chans[i]].user_limite == 0))
                        {
                            chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                            chan_map[chans[i]].nbr_member++;
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
                    if (key[i] == chan_map[chans[i]].chan_password)
                    {
                        chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                        chan_map[chans[i]].nbr_member++;
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
