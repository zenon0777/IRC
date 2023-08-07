#include "ser.hpp"

int server::server_socket()
{
    struct addrinfo seraddr, *res;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.ai_family = AF_UNSPEC;
    seraddr.ai_socktype = SOCK_STREAM;
    seraddr.ai_flags = AI_PASSIVE;
    int o = 1;
    getaddrinfo(NULL, _port, &seraddr, &res);
    int sfd;
    if ((sfd = socket(res->ai_family, SOCK_STREAM, res->ai_protocol)) == -1)
    {
        perror("Socket :error");
        exit(1);
    }
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o)) == -1){
        perror("Socketopt :error");
        exit(1);
    }
    fcntl(sfd, F_SETFL, O_NONBLOCK);
    if (bind(sfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("bind : error");
        exit(1);
    }
    if (listen(sfd, 10) == -1)
    {
        perror("listen :error");
        exit(1);
    }
    return sfd;
}

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
    {
        return "";}
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

int server::server_setup()
{
    struct sockaddr_storage client;
    int cfd;
    char buff[256] = {0};
    socklen_t addr_len;
    int nfds = 1;
    //get socket fd
    set_sfd(server_socket());
    pfds.push_back((struct pollfd){sfd, POLLIN, nfds});
    while (true)
    {
        int poll_res = poll(pfds.data(), pfds.size(), -1);
        if (poll_res == -1)
        {
            perror("Error : Poll");
            break;
        }
        if (pfds[0].revents & POLLIN)
        {
            addr_len = sizeof(client);
            if ((cfd = accept(sfd, (struct sockaddr *)&client, &addr_len)) == -1)
                perror("eroor");
            pfds.push_back((struct pollfd){cfd, POLLIN, nfds++});
        }
        for (int i = 1; i < pfds.size(); ++i)
        {
            if (pfds[i].revents & POLLIN)
            {
                bzero(buff, sizeof(buff));
                size_t nbytes = recv(pfds[i].fd, (void *)buff, sizeof(buff), 0);
                std::cout << "bytes read:" << nbytes << std::endl;
                if (nbytes <= 0)
                {
                    if (nbytes == 0)
                        std::cout << "client close sesion" << std::endl;
                    else
                        perror("error : reciveing mssg");
                    cl.erase(pfds[i].fd);
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                    --i;
                    continue;
                }
                else
                {
                    cl.insert(std::pair<int,Client>(pfds[i].fd, clients));
                    cl.at(pfds[i].fd).set_cfd(pfds[i].fd);
                    cmd_handler(buff, sfd, pfds[i].fd);
                    continue;
                }
                continue;
            }
        }
    }
    close(sfd);
    return 0;
}

// handl user registration i.e PASS pass / USER username :realname / NICK nickname

int server::authenticateClient(std::vector<std::string> c, int c_fd)
{
    if (cl.at(c_fd).get_authent() == true && c[1] != _password)
    {
        cl.at(c_fd).set_authent(false);
        return 2;
    }
    else if (c[1] == _password)
    {
        cl.at(c_fd).set_authent(true);
        return 0;
    }
    else
    {
        cl.at(c_fd).set_authent(false);
        return 2;
    }
    return 1;
}

void server::removeclients(int cfd, int cfd2){
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

bool server::nickname_cmd(std::vector<std::string> &vec, int c_fd)
{
    //just alert the user that the nickname is in use not erase him
    if (is_identical(vec[1], c_fd) == false)
    {
        std::map<int,Client>::iterator it = cl.find(c_fd);
        it->second.set_nickname(trim(vec[1]));
    }
    else
        send(c_fd, "ERROR nickname already in use\r\n", 30, 0);
    //handle identical messg error
    return true;
}

bool server::user_cmd(std::vector<std::string>vec, int c_fd){
    size_t pos = vec[2].find(':');
    std::string r_name;
    if (vec.size() < 3 || *(vec[2].begin()) != ':')
        return false;
    else{
        r_name = vec[2].substr(1);
        cl.at(c_fd).set_username(vec[1], r_name);
        return true;
    }
    return false;
}

channel server::get_channel(std::string name){
    std::map<std::string, channel>::iterator ch;
    ch = chan_map.find(name);
    return ch->second;
}

bool server::send_messg(std::string mssg, int client_fd){
    size_t pos = mssg.find(':');
    const char *buff;
    mssg = mssg.substr(pos);
    buff = mssg.c_str();
    if(send(client_fd, buff, sizeof(buff), 0) < 0)
        return false;
    return true;
}

bool server::handle_recievers(std::vector<std::string> vec, int c_fd){
    std::string nick_names = vec[1];
    std::istringstream ss(nick_names);
    std::string token;
    std::vector<std::string> nicks;
    while (std::getline(ss, token, ',')){
        nicks.push_back(token);}
    int cfd = -1;
    std::vector<std::string>::const_iterator it;
    for(it = nicks.begin(); it!= nicks.end(); it++){
        cfd = get_clientfd(*it);
        std::cout << cfd << std::endl;
        if (cfd > 0)
            send_messg(vec[2], cfd);
    }
    return true;
}

bool server::is_operator(std::string chan_name, int client_fd){
    std::vector<int>::const_iterator it;
    std::map<std::string, channel>::const_iterator mit;
    for (mit = chan_map.begin(); mit!= chan_map.end(); ++mit){
        if (mit->first == chan_name){
            for(it=mit->second._operators_fd.begin(); it!= mit->second._operators_fd.end(); ++it){
                if (*it == client_fd)
                    return true;
            }
        }
    }
    return false;
}

std::vector<std::string> splite(std::string str, char delim){
    std::istringstream ss(str);
    std::string token;
    std::vector<std::string> vec;
    while (std::getline(ss, token, delim))
    {
        token = trim(token);
        vec.push_back(token);
    }
    return vec;
}

bool server::engrafiete_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    if (vec.size() > 3)
    {
        send(client_fd, "Too much params\r\n", 18, 0);
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
            send(client_fd, "Invalid channel name : must start by #/&\r\n", 43, 0);
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
                send(client_fd, "this channel is invite only\r\n", 30, 0);
            else if (chan_map[chans[i]].is_inviteonly == false && chan_map[chans[i]].is_limited)
            {
                if (chan_map[chans[i]].nbr_member < chan_map[chans[i]].user_limite)
                {
                    chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                    chan_map[chans[i]].nbr_member++;
                }
                else
                    send(client_fd, "This channel reach max of members\r\n", 35, 0);
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
                    send(client_fd, "this channel is invite only\r\n", 30, 0);
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
                            send(client_fd, "This channel reach max of members\r\n", 36, 0);
                    }
                    else
                        send(client_fd, "Invalid Key\r\n", 14, 0);
                }
                else
                {
                    if (key[i] == chan_map[chans[i]].chan_password)
                    {
                        chan_map[chans[i]].add_member(cl.at(client_fd), chans[i]);
                        chan_map[chans[i]].nbr_member++;
                    }
                    else
                        send(client_fd, "Invalid Key\r\n", 14, 0);
                }
            }
            else
                send(client_fd, "Need Key to join the channel\r\n", 31, 0);
        }
    }
    return true;
}

bool server::invite_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    int cfd = get_clientfd(vec[1]);
    if (cfd < 0)
        send(client_fd, "Client doesnt exist\r\n", 22, 0);
    else if (is_channelexist(vec[2]) == false && cfd > 0)
    {
        if (channels.add_channel(vec[2], cl.at(cfd), false) == true)
            chan_map[vec[2]] = channels;
    }
    else if (is_channelexist(vec[2]) == true && cfd > 0)
    {
        if (chan_map[vec[2]].is_member(client_fd, vec[2]) == false)
        {
            send(client_fd, "You are not a member\r\n", 22, 0);
            return false;
        }
        else if (chan_map[vec[2]].is_member(cfd, vec[2]) == true)
        {
            send(client_fd, "the client you tried to invite is already a member\r\n", 53, 0);
            return false;
        }
        // it's not an invite only channel every member can invite others
        else if (is_operator(vec[2], client_fd) == false && chan_map[vec[2]].is_inviteonly == false)
        {
            if (chan_map[vec[2]].is_limited == false)
            {
                chan_map[vec[2]].add_member(cl.at(cfd), vec[2]);
                chan_map[vec[2]].nbr_member++;
                send(cfd, "You are now a member ... \r\n", 28, 0);
            }
            else if (chan_map[vec[2]].is_limited == true)
            {
                if (chan_map[vec[2]].user_limite == chan_map[vec[2]].nbr_member)
                    send(client_fd, "Channel reach max of members\r\n", 31, 0);
                else
                {
                    chan_map[vec[2]].add_member(cl.at(cfd), vec[2]);
                    chan_map[vec[2]].nbr_member++;
                    send(cfd, "You are now a member ... \r\n", 28, 0);
                }
            }
        }
        // it's invite only channel operator privs needed
        else if (is_operator(vec[2], client_fd) == true)
        {
            if (chan_map[vec[2]].is_limited == false)
            {
                chan_map[vec[2]].add_member(cl.at(cfd), vec[2]);
                chan_map[vec[2]].nbr_member++;
                send(cfd, "You are now a member ... \r\n", 28, 0);
            }
            else if (chan_map[vec[2]].is_limited == true)
            {
                if (chan_map[vec[2]].user_limite == chan_map[vec[2]].nbr_member)
                    send(client_fd, "Channel reach max of members\r\n", 31, 0);
                else
                {
                    chan_map[vec[2]].add_member(cl.at(cfd), vec[2]);
                    chan_map[vec[2]].nbr_member++;
                    send(cfd, "You are now a member ... \r\n", 28, 0);
                }
            }
        }
        else
        {
            send(client_fd, "You dont have privilige to invite other member\r\n", 49, 0);
            return false;
        }
    }
    return true;
}

bool server::cmd_handler(char *buff, int sfd, int client_fd)
{
    std::string cmd(buff);
    cmd = trim(cmd);
    std::istringstream ss(cmd);
    std::string token;
    std::vector<std::string> vec;
    bool auth = cl.at(client_fd).get_authent();
    while (std::getline(ss, token, ' '))
    {
        token = trim(token);
        vec.push_back(token);
    }
    if (!vec.empty() && vec[0] == "PASS")
    {
        if (vec.size() != 2)
          {
            std::cout << "eeeeooopds\n";
            send(client_fd, "Need more params : Password\r\n", strlen("Need more params : Password\r\n"), 0);
            return false;
        }
        // if the client already auth and ALREADYREGISTRED then trow => ERR_ALREADYREGISTRED
        int flag = authenticateClient(vec, client_fd);
        if (flag == 2)
            send(client_fd, "Invalid Password:...\r\n", 23, 0);
        else if (flag == 0)
            send(client_fd, "Authentificated:...\r\n", 22, 0);
    }
    else if (vec.size() >= 2)
    {
        // check params => NICK <nickname> 
        if (vec[0] == "NICK" && auth == true)
            nickname_cmd(vec, client_fd);
        // check params => USER <username> :<realname ..>
        else if (vec[0] == "USER" && auth == true)
            user_cmd(vec, client_fd);
        
        else if (vec[0] == "PRIVMSG" && auth == true)
            handle_recievers(vec, client_fd);
        
        else if (vec[0] == "JOIN" && auth== true)
        {
            if (engrafiete_sto_kanali(vec, client_fd) == false)
                return false;
        }
        // INVITE <nickname> <#channel_name> => size of vec = 3 exact

        else if (vec[0] == "INVITE" && auth == true)
            invite_sto_kanali(vec, client_fd);
        
        else if (vec[0] == "KICK" && auth ==  true)
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
                    else if (chan_map[vec[1]].is_member(cfd, vec[1]) == false)
                    {
                        send(client_fd, "USer not a member of the channel\r\n", 35, 0);
                        return false;
                    }
                    else if (chan_map[vec[1]].is_member(cfd, vec[1]) == true)
                    {
                        chan_map[vec[1]].remove_member(cl.at(cfd), vec[1]);
                        send(cfd, "you've been kicked from the channel\r\n", 38, 0);
                    }
                }
                else
                    send(client_fd, "You Dont have privilige\r\n", 25, 0);
            }
            else
                send(client_fd, "Channel deosnt exist\r\n", 23, 0);
        }

        //topic #chan_name topic / TOPIC #chan_name => size of vec = 3 max
        else if (vec[0] == "TOPIC" && auth == true)
        {
            if (is_channelexist(vec[1]) == true)
            {
                // if TOPIC #chan_name list the topic even if its not an operator
                if (chan_map[vec[1]].is_topicated == true)
                {
                    if (chan_map[vec[1]].is_member(client_fd, vec[1]) ==  false)
                    {
                        send(client_fd, "You are not a member\r\n", 23, 0);
                        return false;
                    }
                    chan_map[vec[1]].change_topic(client_fd, vec);
                }
                else if (is_operator(vec[1], client_fd) == true)
                    chan_map[vec[1]].change_topic(client_fd, vec);
                else if(is_operator(vec[1], client_fd) == false && chan_map[vec[1]].is_topicated == false)
                    send(client_fd, "You Dont have privilige\r\n", 25, 0);
                else
                    send(client_fd, "You are not a member\r\n", 23, 0);
            }
            else 
                send(client_fd, "Channel deosnt exist\r\n", 23, 0);
        }
        // handle operator cmds MODE #chan-name +-it / MODE #chan-name +-lok one-param
        if (vec[0] == "MODE" && auth == true)
        {
            //parse the vector
            // check if channel exist
            if (is_channelexist(vec[1]) == true){
                if (is_operator(vec[1], client_fd))
                {
                    if (*(vec[2].begin()) == '+')
                    {
                        if (vec[2] == "+o")
                        {
                            int cfd = get_clientfd(vec[3]);
                            if (chan_map[vec[1]].is_member(cfd, vec[1]) == true){
                                send(cfd, "You are now an operator\r\n", 26, 0);
                                chan_map[vec[1]]._operators_fd.push_back(cfd);
                            }
                            else if (cfd < 0 || chan_map[vec[1]].is_member(cfd, vec[1]) == false)
                                send(client_fd, "User doesn't exist or not in channel\r\n", 39, 0);
                        }
                        else if (vec[2] == "+k")
                        {
                            chan_map[vec[1]].secure = true;
                            chan_map[vec[1]].chan_password = vec[3];
                        }
                        else if (vec[2] == "+l"){
                            chan_map[vec[1]].is_limited = true;
                            chan_map[vec[1]].user_limite = atoi(vec[3].c_str());
                        }
                        else{
                            std::cout << "Here\r\n";
                            chan_map[vec[1]].add_mode(client_fd, vec);}
                    }
                    // else if (*(vec[2].begin()) == '-'){
                    //     chan_map[vec[1]].take_mode(client_fd, vec);
                    // }
                }
            }
            else
                send(client_fd, "Channel deosnt exist\r\n", 23, 0);
        }
        // if (vec[0] == "INVITE" && auth == true){
        //     if (is_clientinchannel(client_fd, vec) == true){
        //     }
        // }
        else if (auth == false)
            send(client_fd, "Need authentification to continue:...\r\n", 40, 0);
    }
    return true;
}

int server::get_clientfd(std::string name)
{
    std::map<int, Client>::const_iterator it;
    for (it = cl.cbegin(); it!= cl.cend();++it){
        // std::cout << it->first << "  :::::  " << it->second.get_nickname() << std::endl;
        if (it->second.get_nickname().compare(name) == 0)
            return it->first;
    }
    return -1;
}

bool server::is_channelexist(std::string name){
    // std::cout << "printing map [" << chan_map << "]\r\n";
    std::map<std::string, channel>::iterator mit;
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

server::server(const char *port, const char *password)
{
    _port = port;
    _password = password;
}
server::~server()
{
}


int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        server sever(argv[1], argv[2]);
        sever.server_setup();
    }
    return 0;
}
