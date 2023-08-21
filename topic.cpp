#include"ser.hpp"

bool server::topicate_channel(std::vector<std::string> vec, int client_fd)
{
    if (vec.size() < 2 || vec[1] ==":")
    {
        std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    else if (is_channelexist(vec[1]) == true)
    {
        if (is_operator(vec[1], client_fd) == false && chan_map[vec[1]]->is_member(client_fd) ==  false)
        {
            std::string err = ":" + cl[client_fd].get_host() + " 442 " + cl[client_fd].get_nickname();
            err += " :You're not on that channel\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        else if (vec.size() == 2)
        {
            if (change_topic(client_fd, vec) == false)
                return false;
        } 
        else if (chan_map[vec[1]]->is_topicated == true)
        {
            if (change_topic(client_fd, vec) == false)
                return false;
        }
        else if (is_operator(vec[1], client_fd) == true)
        {
            if (change_topic(client_fd, vec) == false)
                return false;
        }
        else if(is_operator(vec[1], client_fd) == false && chan_map[vec[1]]->is_topicated == false)
        {
            std::string err = ":" + cl[client_fd].get_host() + " 482 " + cl[client_fd].get_nickname();
            err += " :You're not channel operator\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
    }
    else if (is_channelexist(vec[1]) == false)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 403 " + cl[client_fd].get_nickname();
        err += " :No such channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
        return false;
    }
    return true;
}


bool server::change_topic(int cfd, std::vector<std::string> vec)
{
    if (vec.size() == 2 && chan_map[vec[1]]->is_topic == false)
    {
        std::string err = ":" + cl[cfd].get_host() + " 331 " + cl[cfd].get_nickname();
        err += " :No topic is set\r\n";
        const char *buff = err.c_str();
        send(cfd, buff, strlen(buff), 0);
        return false;
    }
    else if (vec.size() == 2 && chan_map[vec[1]]->is_topic == true)
    {
        std::string reply = ":" + cl[cfd].get_host() + " 332 " + cl[cfd].get_nickname();
        reply += " " + vec[1] + " :" + chan_map[vec[1]]->topic + "\r\n";
        const char *buff = reply.c_str();
        send(cfd, buff, strlen(buff), 0);
    }
    else if (vec.size() == 3)
    {
        chan_map[vec[1]]->is_topic = true;
        chan_map[vec[1]]->topic = vec[2];
        std::string reply = ":" + cl[cfd].get_host() + " 332 " + cl[cfd].get_nickname();
        reply += " " + vec[1] + " :" + chan_map[vec[1]]->topic + "\r\n";
        const char *buff = reply.c_str();
        send(cfd, buff, strlen(buff), 0);
    }
    else if (vec.size() == 1)
    {
        std::string err = ":" + cl.at(cfd).get_host() + " 461 " + cl.at(cfd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(cfd, buff, strlen(buff), 0);
        return false;
    }
    return true;
}