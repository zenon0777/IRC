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
                if (nbytes <= 0)
                {
                    if (nbytes == 0)
                        std::cout << "client close sesion" << std::endl;
                    else
                        perror("error : reciveing mssg");
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
                    // std::map<std::string, channel>::iterator it = chan_map.begin();
                    // for(it; it != chan_map.end(); it++)
                    // {std::cout << it->second._chan_name <<std::endl;
                    //     std::vector<int> ls = it->second.clients_fd;
                    //     for (int i = 0; i < ls.size(); i++)
                    //         std::cout << ls[i] << std::endl;
                    // }
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
                removeclients(c_fd, m_it->first);
                cl.erase(c_fd);
                cl.erase(m_it->first);
                return true;
            }
        }
    }
    return false;
}

bool server::nickname_cmd(std::vector<std::string> &vec, int c_fd)
{
    if (is_identical(vec[1], c_fd) == false)
    {
        std::map<int,Client>::iterator it = cl.find(c_fd);
        it->second.set_nickname(vec[1]);
    }
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
    if (vec.size() >= 2){
        if (vec[0] == "NICK" && auth == true)
            nickname_cmd(vec, client_fd);
        else if (vec[0] == "USER" && auth == true)
            user_cmd(vec, client_fd);
        //handle channel cmd; channel(id, channel-name, clients-obj)
        if (vec[0] == "JOIN" && auth== true)
        {
            if (is_channelexist(vec[1]) == false)
            {
                if (channels.add_channel(vec, cl.at(client_fd)) == true)
                    chan_map.insert(std::pair<std::string, channel>(vec[1], channels));
                else
                    send(client_fd, "channel not exist key not neccessaire", 38, 0);
            }
            else
            {

            }
        }
        // handle operator cmds
        // if (vec[0] == "INVITE" && auth == true){
        //     if (get_clientfd() != 0)
        //         if (is_clientinchannel(client_fd, vec) == true)
        // }
        else
            send(client_fd, "Need authentification to continue:...\n", 39, 0);
    }
    return true;
}

bool server::is_channelexist(std::string name){
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
