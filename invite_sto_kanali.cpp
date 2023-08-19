#include "ser.hpp"

bool server::invite_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    if (vec.size() < 3)
    {
        std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
        return false;
    }
    int cfd = get_clientfd(vec[1]);
    if (cfd < 0)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 401 " + cl[client_fd].get_nickname() + " " + vec[1] + " :No such nick/channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    else if (is_channelexist(vec[2]) == false && cfd > 0)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 403 " + cl[client_fd].get_nickname() + " " + vec[2] + " :No such channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    else if (is_channelexist(vec[2]) == true && cfd > 0)
    {
        if (chan_map[vec[2]]->is_member(client_fd) == false && chan_map[vec[2]]->is_operator(client_fd) == false)
        {
            std::string err = ":" + cl[client_fd].get_host() + " 442 " + cl[client_fd].get_nickname();
            err += " :You're not on that channel\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        else if (chan_map[vec[2]]->is_member(cfd) == true || is_operator(vec[2], cfd) == true)
        {
            std::string err = ":" + cl[client_fd].get_host() + " 443 " + cl[client_fd].get_nickname() + " " + cl[cfd].get_nickname()\
            + " " + vec[2];
            err += " :is already on channel\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        else if (is_operator(vec[2], client_fd) == false && chan_map[vec[2]]->is_inviteonly == false)
        {
            if (chan_map[vec[2]]->is_limited == false)
            {
                chan_map[vec[2]]->invited_members.push_back(cfd);
                std::string err = ":" + cl[client_fd].get_nickname() + "!~" + cl[client_fd].get_username() + "@" + cl[client_fd].get_clientip()\
                + " INVITE " + cl[cfd].get_nickname() + " :";
                err += vec[2] + "\r\n";
                const char *buff = err.c_str();
                send(cfd, buff, strlen(buff), 0);
                std::string rpl = ":" + cl[client_fd].get_host() + " 341 " + cl[client_fd].get_nickname() + " " + cl[cfd].get_nickname();
                rpl += " " + vec[2] + "\r\n";
                const char *rp = rpl.c_str();
                send(client_fd, rp, strlen(rp), 0);
                rpl = ":" + cl[client_fd].get_host() + " NOTICE @" + vec[2] + " :" + cl[client_fd].get_nickname() + " invited " + cl[cfd].get_nickname();
                rpl += " into channel " + vec[2] + "\r\n";
                rp = rpl.c_str();
                send(client_fd, rp, strlen(rp), 0);
            }
            else if (chan_map[vec[2]]->is_limited == true)
            {
                if (chan_map[vec[2]]->user_limite == chan_map[vec[2]]->nbr_member)
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                    err += " " + vec[2] + " :Cannot join channel (+l)\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                }
                else
                {
                    chan_map[vec[2]]->invited_members.push_back(cfd);

                    std::string err = ":" + cl[client_fd].get_nickname() + "!~" + cl[client_fd].get_username() + "@" + cl[client_fd].get_clientip()\
                    + " INVITE " + cl[cfd].get_nickname() + " :";
                    err += vec[2] + "\r\n";
                    const char *buff = err.c_str();
                    send(cfd, buff, strlen(buff), 0);
                    // who invite
                    std::string rpl = ":" + cl[client_fd].get_host() + " 341 " + cl[client_fd].get_nickname() + " " + cl[cfd].get_nickname();
                    rpl += " " + vec[2] + "\r\n";
                    const char *rp = rpl.c_str();
                    send(client_fd, rp, strlen(rp), 0);
                    rpl = ":" + cl[client_fd].get_host() + " NOTICE @" + vec[2] + " :" + cl[client_fd].get_nickname() + " invited " + cl[cfd].get_nickname();
                    rpl += " into channel " + vec[2] + "\r\n";
                    rp = rpl.c_str();
                    send(client_fd, rp, strlen(rp), 0);
                }
            }
        }
        // it's invite only channel operator privs needed
        else if (is_operator(vec[2], client_fd) == true)
        {
            if (chan_map[vec[2]]->is_limited == false)
            {
                chan_map[vec[2]]->invited_members.push_back(cfd);

                std::string err = ":" + cl[client_fd].get_nickname() + "!~" + cl[client_fd].get_username() + "@" + cl[client_fd].get_clientip()\
                + " INVITE " + cl[cfd].get_nickname() + " :";
                err += vec[2] + "\r\n";
                const char *buff = err.c_str();
                send(cfd, buff, strlen(buff), 0);
                // who invite
                std::string rpl = ":" + cl[client_fd].get_host() + " 341 " + cl[client_fd].get_nickname() + " " + cl[cfd].get_nickname();
                rpl += " " + vec[2] + "\r\n";
                const char *rp = rpl.c_str();
                send(client_fd, rp, strlen(rp), 0);
                rpl = ":" + cl[client_fd].get_host() + " NOTICE @" + vec[2] + " :" + cl[client_fd].get_nickname() + " invited " + cl[cfd].get_nickname();
                rpl += " into channel " + vec[2] + "\r\n";
                rp = rpl.c_str();
                send(client_fd, rp, strlen(rp), 0);
            }
            else if (chan_map[vec[2]]->is_limited == true)
            {
                if (chan_map[vec[2]]->user_limite == chan_map[vec[2]]->nbr_member)
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 471 " + cl[client_fd].get_nickname();
                    err += " " + vec[2] + " :Cannot join channel (+l)\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                }
                else
                {
                    chan_map[vec[2]]->invited_members.push_back(cfd);

                    std::string err = ":" + cl[client_fd].get_nickname() + "!~" + cl[client_fd].get_username() + "@" + cl[client_fd].get_clientip()\
                    + " INVITE " + cl[cfd].get_nickname() + " :";
                    err += vec[2] + "\r\n";
                    const char *buff = err.c_str();
                    send(cfd, buff, strlen(buff), 0);
                    // who invite
                    std::string rpl = ":" + cl[client_fd].get_host() + " 341 " + cl[client_fd].get_nickname() + " " + cl[cfd].get_nickname();
                    rpl += " " + vec[2] + "\r\n";
                    const char *rp = rpl.c_str();
                    send(client_fd, rp, strlen(rp), 0);
                    rpl = ":" + cl[client_fd].get_host() + " NOTICE @" + vec[2] + " :" + cl[client_fd].get_nickname() + " invited " + cl[cfd].get_nickname();
                    rpl += " into channel " + vec[2] + "\r\n";
                    rp = rpl.c_str();
                    send(client_fd, rp, strlen(rp), 0);
                }
            }
        }
        else
        {
            std::string err = ":" + cl[client_fd].get_host() + " 482 " + cl[client_fd].get_nickname();
            err += " :You're not channel operator\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
    }
    return true;
}