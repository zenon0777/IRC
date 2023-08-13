#include"ser.hpp"

void server::oper_rply(std::string name, int oper, int cfd, std::string mssg)
{
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + mssg + cl[cfd].get_nickname() + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (int j =0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + mssg + cl[cfd].get_nickname() + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }
}

void server::add_reply(std::string name, int cfd, std::string mssg)
{
    //:losf!~sd@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +i 
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
        notif += ".ip MODE " + name + mssg + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (int j =0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
        notif += ".ip MODE " + name + " +o " + cl[cfd].get_nickname() + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }

}

void server::take_reply(std::string name, int cfd, std::string mssg)
{
    //:losf!~sd@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +i 
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
        notif += ".ip MODE " + name + mssg + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (int j =0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[cfd].get_nickname() + "!~" + cl[cfd].get_username() + "@" + cl[cfd].get_clientip();
        notif += ".ip MODE " + name + mssg + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }

}

// :los!~k@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +o lop

// :losf!~sd@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +k key

// :losf!~sd@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +l 5


void server::limite_reply(std::string name, int oper, std::string limite)
{
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + " +l  " + limite + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (int j =0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + " +l  " + limite + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }
}

void server::key_reply(std::string name, int oper, std::string mssg)
{
    std::vector<int> fds = chan_map[name]->get_chan_member();
    for (int i = 0; i < fds.size(); i++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + mssg + chan_map[name]->chan_password + "\r\n";
        const char *rpl = notif.c_str();
        send(fds[i], rpl, strlen(rpl), 0);
    }
    std::vector<int> opers = chan_map[name]->_operators_fd;
    std::string chan_opers;
    for (int j =0; j < opers.size(); j++)
    {
        std::string notif = ":" + cl[oper].get_nickname() + "!~" + cl[oper].get_username() + "@" + cl[oper].get_clientip();
        notif += ".ip MODE " + name + mssg + chan_map[name]->chan_password + "\r\n";
        const char *rpl = notif.c_str();
        send(opers[j], rpl, strlen(rpl), 0);
    }
}

void server::take_opers(std::vector<std::string> vec, int client_fd)
{
    int cfd = get_clientfd(vec[3]);
    // if isoperato already
    if (cfd < 0)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 401 " + cl[client_fd].get_nickname() + vec[3] + " :No such nick/channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    if (chan_map[vec[1]]->is_operator(vec[1], cfd) == true && chan_map[vec[1]]->is_operator(vec[1], client_fd))
    {
        std::vector<int>::iterator vit;
        for (vit = chan_map[vec[1]]->_operators_fd.begin(); vit != chan_map[vec[1]]->_operators_fd.end(); ++vit)
        {
            if (*vit == cfd)
            {
                chan_map[vec[1]]->_operators_fd.erase(vit);
                break;
            }
        }
        chan_map[vec[1]]->clients_fd.push_back(cfd);
        oper_rply(vec[1], client_fd, cfd, " -o ");
    }
    else if (cfd < 0 || (chan_map[vec[1]]->is_member(cfd, vec[1]) == false && \
    chan_map[vec[1]]->is_operator(vec[1], cfd) == false))
    {
        // :punch.wa.us.dal.net 441 losp posx #top :They aren't on that channel
        std::string err = ":" + cl[client_fd].get_host() + " 441 " + cl[client_fd].get_nickname() + vec[3] + vec[1] \
        + " :They aren't on that channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    else if (chan_map[vec[1]]->is_operator(vec[1], client_fd) == false)
    {
        std::string err = ":" + cl[cfd].get_host() + " 482 " + cl[cfd].get_nickname();
        err += " :You're not channel operator\r\n";
        const char *buff = err.c_str();
        send(cfd, buff, strlen(buff), 0);
        return ;
    }
}

void server::add_opers(std::vector<std::string> vec, int client_fd)
{
    // to everyone in channel : :los!~k@5c8c-aff4-7127-3c3-1c20.230.197.ip MODE #top +o lop
    int cfd = get_clientfd(vec[3]);
    if (cfd < 0)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 401 " + cl[client_fd].get_nickname() + vec[3] + " :No such nick/channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    if (chan_map[vec[1]]->is_member(cfd, vec[1]) == true && chan_map[vec[1]]->is_operator(vec[1], client_fd))
    {
        std::vector<int>::iterator vit;
        for (vit = chan_map[vec[1]]->clients_fd.begin(); vit != chan_map[vec[1]]->clients_fd.end(); ++vit)
        {
            if (*vit == cfd)
            {
                chan_map[vec[1]]->clients_fd.erase(vit);
                break;
            }
        }
        chan_map[vec[1]]->_operators_fd.push_back(cfd);
        oper_rply(vec[1], client_fd, cfd, " +o ");
    }
    else if (cfd < 0 || (chan_map[vec[1]]->is_member(cfd, vec[1]) == false && \
    chan_map[vec[1]]->is_operator(vec[1], cfd) == false))
    {
        std::string err = ":" + cl[client_fd].get_host() + " 441 " + cl[client_fd].get_nickname() + vec[3] + vec[1] \
        + " :They aren't on that channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
    }
    else if (chan_map[vec[1]]->is_operator(vec[1], client_fd) == false)
    {
        std::string err = ":" + cl[cfd].get_host() + " 482 " + cl[cfd].get_nickname();
        err += " :You're not channel operator\r\n";
        const char *buff = err.c_str();
        send(cfd, buff, strlen(buff), 0);
        return ;
    }
}

void server::take_pass(std::vector<std::string> vec, int client_fd)
{
    key_reply(vec[1], client_fd, " -k ");
    chan_map[vec[1]]->secure = false;
    chan_map[vec[1]]->chan_password = "";
}

bool server::take_mode(int cfd, std::vector<std::string> vec)
{
    if (chan_map[vec[1]]->is_operator(vec[1], cfd) == true)
    {
        std::string s = vec[2];
        for (int i =1; i < s.length(); i++)
        {
            if (s[i] == 'i')
            {
                chan_map[vec[1]]->is_inviteonly = false;
                take_reply(vec[1], cfd, " -i ");
            }
            if (s[i] == 'l')
            {
                chan_map[vec[1]]->is_limited = false;
                chan_map[vec[1]]->user_limite = 0;
                take_reply(vec[1], cfd, " -l ");
            }
            if (s[i] == 't')
            {
                chan_map[vec[1]]->is_topicated = false;
                take_reply(vec[1], cfd, " -t ");
            }
            else
                return false;
        }
    }
    else
    {
        std::string err = ":" + cl[cfd].get_host() + " 482 " + cl[cfd].get_nickname();
        err += " :You're not channel operator\r\n";
        const char *buff = err.c_str();
        send(cfd, buff, strlen(buff), 0);
        return false;
    }
    return true;
}

bool server::add_mode(int cfd, std::vector<std::string> vec)
{
    if (chan_map[vec[1]]->is_operator(vec[1], cfd) == true)
    {
        std::string s = vec[2];
        for (int i =1; i < s.length(); i++)
        {
            if (s[i] == 'i')
            {
                chan_map[vec[1]]->is_inviteonly = true;
                add_reply(vec[1], cfd, " +i ");
            }
            if (s[i] == 't')
            {
                chan_map[vec[1]]->is_topicated = true;
                add_reply(vec[1], cfd, " +t ");
            }
            else
                return false;
        }
    }
    return true;
}


//// MAIN FUNCTION OF MODE CMD

void server::mode_change(std::vector<std::string> vec, int client_fd)
{
    if (is_channelexist(vec[1]) == true)
    {
        if (is_operator(vec[1], client_fd))
        {
            if (*(vec[2].begin()) == '+')
            {
                if (vec[2] == "+o")
                {
                    add_opers(vec, client_fd);
                }
                else if (vec[2] == "+k")
                {
                    chan_map[vec[1]]->secure = true;
                    chan_map[vec[1]]->chan_password = vec[3];
                    key_reply(vec[1], client_fd, " +k ");
                }
                else if (vec[2] == "+l")
                {
                    chan_map[vec[1]]->is_limited = true;
                    chan_map[vec[1]]->user_limite = atoi(vec[3].c_str());
                    limite_reply(vec[1], client_fd, vec[3]);
                }
                else
                {
                    add_mode(client_fd, vec);
                }
            }
            else if (*(vec[2].begin()) == '-')
            {
                if (vec[2] == "-o")
                    take_opers(vec, client_fd);
                else if (vec[2] == "-k")
                    take_pass(vec, client_fd);
                take_mode(client_fd, vec);
            }
        }
    }
    else if (is_channelexist(vec[1]) == false)
    {
        std::string err = ":" + cl[client_fd].get_host() + " 403 " + cl[client_fd].get_nickname() + " " + vec[2] + " :No such channel\r\n";
        const char *buff = err.c_str();
        send(client_fd, buff, strlen(buff), 0);
        return ;
    }
    return ;
}