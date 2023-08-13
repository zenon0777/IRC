#include"ser.hpp"

bool server::kick_memeber(std::vector<std::string> vec, int client_fd)
{
    if (is_channelexist(vec[1]) == true)
    {
        if (is_operator(vec[1], client_fd) ==  true)
        {
            int cfd = get_clientfd(vec[2]);
            if (cfd < 0)
            {
                send(client_fd, "Client doesnt exist\r\n", 22, 0);
                return false;
            }
            else if (chan_map[vec[1]]->is_member(cfd, vec[1]) == false)
            {
                send(client_fd, "USer not a member of the channel\r\n", 35, 0);
                return false;
            }
            else if (chan_map[vec[1]]->is_member(cfd, vec[1]) == true)
            {
                chan_map[vec[1]]->remove_member(cl.at(cfd), vec[1]);
                send(cfd, "you've been kicked from the channel\r\n", 38, 0);
            }
            else if (is_operator(vec[1], cfd) == true)
                send(client_fd, "The client you tried to kiked off is an operator: cant remove him\r\n", strlen("The client you tried \
                to kiked off is an operator: cant remove him\r\n"), 0);
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