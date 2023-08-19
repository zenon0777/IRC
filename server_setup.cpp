#include"ser.hpp"

int server::server_socket()
{
    struct addrinfo seraddr, *res;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.ai_family = AF_INET;
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
    short int nfds = 1;
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
        for (size_t i = 0; i < pfds.size(); ++i)
        {
            if (pfds[i].revents & POLLIN)
            {
                int client_family = client.ss_family;
                const char *ip_address = inet_ntop(client_family, get_in_addr((struct sockaddr*)&client), remoteIP, INET_ADDRSTRLEN);
                if (ip_address == NULL) {
                    perror("inet_ntop");
                }
                char client_hostname[NI_MAXHOST];
                int result = getnameinfo((struct sockaddr*)&client, addr_len, client_hostname, NI_MAXHOST, NULL, 0, 0);
                if (result != 0) {
                    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(result));
                    strncpy(client_hostname, remoteIP, NI_MAXHOST);
                }
                // std::cout << "Client connected from IP: " << ip_address << " Hostname: " << client_hostname << std::endl;
                bzero(buff, sizeof(buff));
                size_t nbytes = recv(pfds[i].fd, (void *)buff, sizeof(buff), 0);
                std::cout << "bytes read:" << nbytes << std::endl;
                if (nbytes <= 0)
                {
                    // for more optimization we should add signal handler section
                    
                    // :lop!~s@5c8c-aff4-7127-3c3-1c20.230.197 QUIT :Client closed connection
                    // ERROR :Closing Link: 197.230.30.146 (Client closed connection)
                    if (nbytes == 0)
                    {
                        /// a traiter
                        if (!cl.empty())
                        {
                            std::string quit = ":" + cl[pfds[i].fd].get_nickname() + "!~" + cl[pfds[i].fd].get_username()+"@";
                            quit += cl[pfds[i].fd].get_clientip() + " QUIT :Client closed connection\r\n";
                            const char *buff = quit.c_str();
                            send(pfds[i].fd, buff, strlen(buff), 0);
                        }
                        std::map<std::string, channel*>::iterator it;
                        for (it = chan_map.begin(); it != chan_map.end(); ++it)
                        {
                            if (it->second->_operators_fd.size() == 1)
                            {
                                if (it->second->_operators_fd[0] == pfds[i].fd)
                                {
                                    delete it->second;
                                    chan_map.erase(it);
                                    break;
                                }
                            }
                            if (it->second->clients_fd.size() > 0)
                            {
                                std::vector<int>::iterator vit;
                                for (vit = it->second->_operators_fd.begin(); vit != it->second->_operators_fd.end(); ++vit){
                                    if (*vit == pfds[i].fd)
                                    {
                                        it->second->_operators_fd.erase(vit);
                                        break ;
                                    }
                                }
                                std::vector<int>::iterator cit;
                                for (cit = it->second->clients_fd.begin(); cit != it->second->clients_fd.end(); ++cit){
                                    if (*cit == pfds[i].fd)
                                    {
                                        it->second->clients_fd.erase(cit);
                                        break ;
                                    }
                                }
                            }
                        }
                    }
                    else
                        perror("error : reciveing mssg");
                    cl.erase(pfds[i].fd);
                    close(pfds[i].fd);
                    pfds.erase(pfds.begin() + i);
                    --i;
                    continue;
                }
                else if (nbytes > 0)
                {
                    cl.insert(std::pair<int,Client>(pfds[i].fd, clients));
                    cl.at(pfds[i].fd).set_cfd(pfds[i].fd);
                    cl.at(pfds[i].fd).set_host(client_hostname);
                    cl.at(pfds[i].fd).set_clientip(ip_address);
                    if (nbytes < 256)
                        cmd_handler(buff, pfds[i].fd);
                    continue;
                }
                continue;
            }
        }
    }
    std::map<std::string, channel*>::iterator it;
    for (it= chan_map.begin(); it !=chan_map.end(); ++it)
    {
        delete chan_map[it->first];
    }
    chan_map.clear();
    close(sfd);
    return 0;
}
