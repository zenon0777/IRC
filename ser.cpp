#include "ser.hpp"

int server::server_socket()
{
    struct addrinfo seraddr, *res;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.ai_family = AF_UNSPEC;
    seraddr.ai_socktype = SOCK_STREAM;
    seraddr.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, _port, &seraddr, &res);
    int sfd;
    if ((sfd = socket(res->ai_family, SOCK_STREAM, res->ai_protocol)) == -1)
    {
        perror("Socket :error");
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
                    cmd_handler(buff, sfd, pfds[i].fd);
                    continue;
                }
                continue;
            }
        }
    }
    close(sfd);
}

// handl user registration i.e PASS pass / USER username :realname / NICK nickname

int server::authenticateClient(std::vector<std::string> c, int c_fd)
{
    std::cout << c[1] << std::endl;
    if (clients[c_fd].get_authent() == true && c[1] != _password)
    {
        clients[c_fd].set_authent(false);
        return 2;
    }
    else if (c[1] == _password)
    {
        puts("yes0");
        clients[c_fd].set_authent(true);
        return 0;
    }
    else
    {
        clients[c_fd].set_authent(false);
        return 2;
    }
    return 1;
}

bool server::cmd_handler(char *buff, int sfd, int client_fd)
{
    std::string cmd(buff);
    trim(cmd);
    std::cout << cmd.length() << std::endl;
    std::istringstream ss(cmd);
    std::string token;
    std::vector<std::string> vec;
    while (std::getline(ss, token, ' '))
    {
        vec.push_back(token);
    }

    if (vec[0] == "PASS")
    {
        if (authenticateClient(vec, client_fd) == 2)
            send(client_fd, "Invalid Password:...\n", 22, 0);
        else if (authenticateClient(vec, client_fd) == 0)
            send(client_fd, "Authentificated:...\n", 21, 0);
    }
    if (vec[0] == "NICK")
    {
        if (clients[client_fd].get_authent() == false)
            send(client_fd, "Need to authentificate", 23, 0);
        else
            clients[client_fd].set_nickname(vec[2]);
    }
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
