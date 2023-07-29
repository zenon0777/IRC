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
    char buff[256] = "\0";
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
    pit = pfds.begin();
    for (pit; pit!= pfds.end(); ++pit){
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
    std::map<int, Client>::iterator m_it = cl.begin();
    std::string nickname;

    for(m_it; m_it != cl.end(); ++m_it){
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
        it->second.set_nickname(vec[1]);
    }
    else
        send(c_fd, "ERROR nickname already in use\n", 30, 0);
    //handle identical messg error
}

bool server::user_cmd(std::vector<std::string>vec, int c_fd){
    size_t pos = vec[2].find(':');
    std::string r_name;
    if (vec.size() < 3 || *(vec[2].begin()) != ':')
        return false;
    else{
        r_name = vec[2].substr(1);
        cl.at(c_fd).set_username(vec[1], r_name);
    }
}

channel server::get_channel(std::string name){
    std::map<std::string, channel>::iterator ch;
    ch = chan_map.find(name);
    return ch->second;
}

bool server::send_messg(std::string mssg, int client_fd){
    const char *buff = mssg.c_str();
    int len = mssg.length();
    if(send(client_fd, buff, len, 0) < 0)
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
    for (int i = 0; i <= nicks.size(); i++)
    {
        int cfd = get_clientfd(nicks[i]);
        if (cfd > 0 && c_fd != cfd)
            send_messg(vec[2], cfd);
    }
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
    if (vec.size() == 2 && vec[0] == "PASS")
    {
        //if nick/username seted before mention it 
        int flag = authenticateClient(vec, client_fd);
        if (flag == 2)
            send(client_fd, "Invalid Password:...\n", 22, 0);
        else if (flag == 0)
            send(client_fd, "Authentificated:...\n", 21, 0);
    }
    else if (vec.size() >= 2){
        if (vec[0] == "NICK" && auth == true)
            nickname_cmd(vec, client_fd);
        else if (vec[0] == "USER" && auth == true)
            user_cmd(vec, client_fd);
        //handle channel cmd; channel(id, channel-name, clients-obj)
        else if (vec[0] == "PRIVMSG" && auth == true){
            handle_recievers(vec, client_fd);
        }
        else if (vec[0] == "JOIN" && auth== true)
        {
            // parser 
            if (is_channelexist(vec[1]) == false){
                if (channels.add_channel(vec, cl.at(client_fd)) == true){
                    chan_map.insert(std::pair<std::string, channel>(vec[1], channels));
                }
            }
            else {
                get_channel(vec[1]).add_member(client_fd, vec[1]);
            }
        }
        // PRIVMSG
        // handle operator cmds
        // if (vec[0] == "INVITE" && auth == true){
        //     if (get_clientfd() != 0)
        //         if (is_clientinchannel(client_fd, vec) == true)
        // }
        else if (auth == false)
            send(client_fd, "Need authentification to continue:...\n", 39, 0);
    }
    return true;
}

int server::get_clientfd(std::string name){
    std::map<int, Client>::iterator it;
    for(it = cl.begin();it!=cl.end();++it)
    {
        std::cout << it->first << "  ::  " << it->second.get_nickname() << "  >>>  " << name <<std::endl;
        if (it->second.get_nickname() == name){
            return it->first;}
    }
    return -1;
}

bool server::is_channelexist(std::string name){
    std::cout << "printing map [" << chan_map << "]\n";
    if (chan_map.find(name) != chan_map.end())
        return true;
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
