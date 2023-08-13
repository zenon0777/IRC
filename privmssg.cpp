#include"ser.hpp"

bool server::send_messg(std::string mssg, int client_fd){
    // size_t pos = mssg.find(':');
    const char *buff;
    // mssg = mssg.substr(pos);
    mssg += "\r\n";
    buff = mssg.c_str();
    if(send(client_fd, buff, strlen(buff), 0) < 0)
        return false;
    return true;
}

bool server::handle_recievers(std::vector<std::string> vec, int c_fd){
    std::string nick_names = vec[1];
    std::istringstream ss(nick_names);
    std::string token;
    std::vector<std::string> nicks;
    std::string mssg;
    while (std::getline(ss, token, ',')){
        nicks.push_back(token);}
    int cfd = -1;
    std::vector<std::string>::const_iterator it;
    for (int i = 2; i < vec.size(); i++){
        mssg += vec[i];
        if (i < vec.size() - 1)
            mssg += " ";
    }
    //:punch.wa.us.dal.net 401 polsxf loskdscvfd :No such nick/channel
    for(int i = 0; i < nicks.size(); i++)
    {
        if (*nicks[i].begin() == '#')
        {
            std::vector<int>::iterator vit;
            std::cout << "size  :::  " << chan_map[nicks[i]]->clients_fd.size() << std::endl;
            for(int j = 0; j < chan_map[nicks[i]]->clients_fd.size(); j++)
            {
                std::cout << chan_map[nicks[i]]->clients_fd[j] << std::endl;
                if (chan_map[nicks[i]]->clients_fd[j] > 0)
                    send_messg(mssg, chan_map[nicks[i]]->clients_fd[j]);
                else
                {
                    std::string err = ":" + cl[c_fd].get_host() + " 401 " + cl[c_fd].get_nickname() + nicks[i] + " :No such nick/channel\r\n";
                    const char *buff = err.c_str();
                    send(c_fd, buff, strlen(buff), 0);
                }
            }
            for(int k = 0; k < chan_map[nicks[i]]->_operators_fd.size(); k++)
            {
                std::cout << chan_map[nicks[i]]->_operators_fd[k] << std::endl;
                if (chan_map[nicks[i]]->_operators_fd[k] > 0)
                    send_messg(mssg, chan_map[nicks[i]]->_operators_fd[k]);
                else
                {
                    std::string err = ":" + cl[c_fd].get_host() + " 401 " + cl[c_fd].get_nickname() + nicks[i] + " :No such nick/channel\r\n";
                    const char *buff = err.c_str();
                    send(c_fd, buff, strlen(buff), 0);
                }
            }
        }
        else
        {
            cfd = get_clientfd(nicks[i]);
            std::cout << cfd << std::endl;
            if (cfd > 0)
                send_messg(mssg, cfd);
            else if (cfd < 0)
            {
                std::string err = ":" + cl[c_fd].get_host() + " 401 " + cl[c_fd].get_nickname() + nicks[i] + " :No such nick/channel\r\n";
                const char *buff = err.c_str();
                send(c_fd, buff, strlen(buff), 0);
            }
        }
    }
    return true;
}