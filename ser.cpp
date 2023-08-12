#include "ser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

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

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
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
    char remoteIP[INET6_ADDRSTRLEN];
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
        for (int i = 0; i < pfds.size(); ++i)
        {
            /*int client_family = client.ss_family;
            const char *host = inet_ntop(client_family, get_in_addr((struct sockaddr*)&client), remoteIP, INET6_ADDRSTRLEN);
            char client_hostname[NI_MAXHOST];
            int result = getnameinfo((struct sockaddr*)&client, addr_len, client_hostname, NI_MAXHOST, NULL, 0, 0);
            if (result != 0) {
                fprintf(stderr, "getnameinfo: %s\n", gai_strerror(result));
                strncpy(client_hostname, remoteIP, NI_MAXHOST);
            }*/
            if (pfds[i].revents & POLLIN)
            {
                int client_family = client.ss_family;
                const char *ip_address = inet_ntop(client_family, get_in_addr((struct sockaddr*)&client), remoteIP, INET6_ADDRSTRLEN);
                if (ip_address == nullptr) {
                    perror("inet_ntop");
                }
                char client_hostname[NI_MAXHOST];
                int result = getnameinfo((struct sockaddr*)&client, addr_len, client_hostname, NI_MAXHOST, NULL, 0, 0);
                if (result != 0) {
                    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(result));
                    strncpy(client_hostname, remoteIP, NI_MAXHOST);
                }
                std::cout << "Client connected from IP: " << ip_address << " Hostname: " << client_hostname << std::endl;

                
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
                    cl.at(pfds[i].fd).set_host(client_hostname);
                    cl.at(pfds[i].fd).set_clientip(ip_address);
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
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 433 " + cl.at(c_fd).get_nickname();
        err += " :Nickname is already in use\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
    }
    return true;
}

bool server::user_cmd(std::vector<std::string>vec, int c_fd){
    size_t pos = vec[2].find(':');
    std::string r_name;
    if (vec.size() < 3 || *(vec[2].begin()) != ':')
        return false;
    else
    {
        r_name = vec[2].substr(1);
        cl.at(c_fd).set_username(vec[1], r_name);
        std::cout << cl.at(c_fd).get_username() << std::endl;
        return true;
    }
    return false;
}

//channel server::get_channel(std::string name){
//    std::map<std::string, channel>::iterator ch;
//    ch = chan_map->find(name);
//    return ch->second;
//}

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
    while (std::getline(ss, token, ',')){
        nicks.push_back(token);}
    int cfd = -1;
    std::vector<std::string>::const_iterator it;
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
                    send_messg(vec[2], chan_map[nicks[i]]->clients_fd[j]);
                else
                {
                    std::string mssg = "401 ERR_NOSUCHNICK " + nicks[i] + " :No such nick/channel\r\n";
                    const char *buff = mssg.c_str();
                    send(c_fd, buff, strlen(buff), 0);
                }
            }
        }
        else
        {
            cfd = get_clientfd(nicks[i]);
            std::cout << cfd << std::endl;
            if (cfd > 0)
                send_messg(vec[2], cfd);
        }
    }
    return true;
}

bool server::is_operator(std::string chan_name, int client_fd){
    std::vector<int>::iterator it;
    std::map<std::string, channel *>::iterator mit;
    for (mit = chan_map.begin(); mit!= chan_map.end(); ++mit){
        if (mit->first == chan_name){
            for(it=mit->second->_operators_fd.begin(); it!= mit->second->_operators_fd.end(); ++it){
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

bool server::invite_sto_kanali(std::vector<std::string> vec, int client_fd)
{
    int cfd = get_clientfd(vec[1]);
    if (cfd < 0)
        send(client_fd, "Client doesnt exist\r\n", 22, 0);
    else if (is_channelexist(vec[2]) == false && cfd > 0)
    {
        if (channels->add_channel(vec[2], cl.at(cfd), false) == true)
            chan_map[vec[2]] = channels;
    }
    else if (is_channelexist(vec[2]) == true && cfd > 0)
    {
        if (chan_map[vec[2]]->is_member(client_fd, vec[2]) == false)
        {
            send(client_fd, "You are not a member\r\n", 22, 0);
            return false;
        }
        else if (chan_map[vec[2]]->is_member(cfd, vec[2]) == true)
        {
            send(client_fd, "the client you tried to invite is already a member\r\n", 53, 0);
            return false;
        }
        // it's not an invite only channel every member can invite others
        else if (is_operator(vec[2], client_fd) == false && chan_map[vec[2]]->is_inviteonly == false)
        {
            if (chan_map[vec[2]]->is_limited == false)
            {
                chan_map[vec[2]]->add_member(cl.at(cfd), vec[2]);
                chan_map[vec[2]]->nbr_member++;
                send(cfd, "You are now a member ... \r\n", 28, 0);
            }
            else if (chan_map[vec[2]]->is_limited == true)
            {
                if (chan_map[vec[2]]->user_limite == chan_map[vec[2]]->nbr_member)
                    send(client_fd, "Channel reach max of members\r\n", 31, 0);
                else
                {
                    chan_map[vec[2]]->add_member(cl.at(cfd), vec[2]);
                    chan_map[vec[2]]->nbr_member++;
                    send(cfd, "You are now a member ... \r\n", 28, 0);
                }
            }
        }
        // it's invite only channel operator privs needed
        else if (is_operator(vec[2], client_fd) == true)
        {
            if (chan_map[vec[2]]->is_limited == false)
            {
                chan_map[vec[2]]->add_member(cl.at(cfd), vec[2]);
                chan_map[vec[2]]->nbr_member++;
                send(cfd, "You are now a member ... \r\n", 28, 0);
            }
            else if (chan_map[vec[2]]->is_limited == true)
            {
                if (chan_map[vec[2]]->user_limite == chan_map[vec[2]]->nbr_member)
                    send(client_fd, "Channel reach max of members\r\n", 31, 0);
                else
                {
                    chan_map[vec[2]]->add_member(cl.at(cfd), vec[2]);
                    chan_map[vec[2]]->nbr_member++;
                    send(cfd, "You are now a member ... \r\n", 28, 0);
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

//change topic



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
    if (vec.size() == 2 && chan_map[vec[1]]->is_topic == true)
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
        send(cfd, "ERR_Parameter : ...\n",20, 0);
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
    std::cout << cmd << std::endl;
    if (!vec.empty() && vec[0] == "PASS")
    {
        if (vec.size() < 2)
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
            err += " :Not enough parameters\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        // if the client already auth and ALREADYREGISTRED then trow => ERR_ALREADYREGISTRED
        int flag = authenticateClient(vec, client_fd);
        if (flag == 2)
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 464 " + cl.at(client_fd).get_nickname();
            err += " :password incorrect\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
        }
    }
    else if (vec.size() >= 1)
    {
        // check params => NICK <nickname> 
        if (vec[0] == "NICK")
        {
            if (vec.size() == 1 && vec[0] == "NICK" || (vec[1] == ":" && vec[0] == "NICK"))
            {
                std::string err = ":" + cl.at(client_fd).get_host() + " 431 " + cl.at(client_fd).get_nickname();
                err += " :No nickname given\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
            else if (vec[0] == "NICK" && auth == true)
            {
                std::cout << "nick\n";
                nickname_cmd(vec, client_fd);
            }
        }
        // check params => USER <username> :<realname ..>
        if (vec[0] == "USER" && auth == true)
        {
            std::cout << "user\n";
            user_cmd(vec, client_fd);   
        }
        else if (vec[0] == "PRIVMSG" && auth == true)
            handle_recievers(vec, client_fd);
        
        else if (vec[0] == "JOIN" && auth== true)
        {
            std::cout << "join\n";
            if (engrafiete_sto_kanali(vec, client_fd) == false){
                std::cout << "error join\n";
                return false;
            }
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
        }

        //topic #chan_name topic / TOPIC #chan_name => size of vec = 3 max
        else if (vec[0] == "TOPIC" && auth == true)
        {
            if (vec.size() < 2)
            {
                std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
                err += " :Not enough parameters\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
            if (is_channelexist(vec[1]) == true)
            {
                if (is_operator(vec[1], client_fd) == false && chan_map[vec[1]]->is_member(client_fd, vec[1]) ==  false)
                {
                    std::string err = ":" + cl[client_fd].get_host() + " 442 " + cl[client_fd].get_nickname();
                    err += " :You're not on that channel\r\n\r\n";
                    const char *buff = err.c_str();
                    send(client_fd, buff, strlen(buff), 0);
                    return false;
                }
                if (chan_map[vec[1]]->is_topicated == true)
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
            else
            {
                std::string err = ":" + cl[client_fd].get_host() + " 403 " + cl[client_fd].get_nickname();
                err += " :No such channel\r\n";
                const char *buff = err.c_str();
                send(client_fd, buff, strlen(buff), 0);
            }
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
                            if (chan_map[vec[1]]->is_member(cfd, vec[1]) == true){
                                send(cfd, "You are now an operator\r\n", 26, 0);
                                chan_map[vec[1]]->_operators_fd.push_back(cfd);
                            }
                            else if (cfd < 0 || chan_map[vec[1]]->is_member(cfd, vec[1]) == false)
                                send(client_fd, "User doesn't exist or not in channel\r\n", 39, 0);
                        }
                        else if (vec[2] == "+k")
                        {
                            chan_map[vec[1]]->secure = true;
                            chan_map[vec[1]]->chan_password = vec[3];
                        }
                        else if (vec[2] == "+l"){
                            chan_map[vec[1]]->is_limited = true;
                            chan_map[vec[1]]->user_limite = atoi(vec[3].c_str());
                        }
                        else{
                            std::cout << "Here\r\n";
                            chan_map[vec[1]]->add_mode(client_fd, vec);}
                    }
                    // else if (*(vec[2].begin()) == '-'){
                    //     chan_map[vec[1]]->take_mode(client_fd, vec);
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
    std::map<std::string, channel *>::iterator mit;
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
