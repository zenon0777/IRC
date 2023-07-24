#include"ser.hpp"

int server::server_socket(){
    struct addrinfo seraddr, *res;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.ai_family = AF_UNSPEC;
    seraddr.ai_socktype = SOCK_STREAM;
    seraddr.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, _port, &seraddr, &res);
    int sfd;
    if (sfd = socket(res->ai_family, SOCK_STREAM, res->ai_socktype) == -1)
    {
        perror("Socket :error");
        exit(1);}
    if (bind(sfd, res->ai_addr, res->ai_addrlen) ==-1){
        perror("bind : error");
        exit(1);} 
    if (listen(sfd, 10) == -1){
        perror("listen :error");
        exit(1);}
    return sfd;
}

int server::server_setup(){
    struct sockaddr_storage client;
    int cfd;
    char buff[256];
    socklen_t addr_len;  
    int nfds = 1;
    set_sfd(server_socket());
    pfds.push_back((struct pollfd){sfd, POLL_IN, nfds});
    while(true){
        int poll_res = poll(pfds.data(), pfds.size(), -1);
        if (poll_res == -1)
        {
            perror("Error : Poll");
            exit(1);
        }
        for (int i = 0; i < nfds; i++){
            if (pfds[i].revents && POLL_IN){
                if (pfds[i].fd == sfd){
                    addr_len = sizeof(client);
                    if (cfd = accept(sfd, (struct sockaddr *)&client, &addr_len) == -1)
                        perror ("eroor");
                    pfds.push_back((struct pollfd){cfd, POLL_IN, nfds++});
                }
                else {
                    size_t nbytes =  recv(pfds[i].fd, (void *)buff, sizeof(buff), 0);
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                            std::cout << "client close sesion" << std::endl;
                        else
                            perror("error : reciveing mssg");
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin() + i);
                        --i;
                    }
                    else{
                        cmd_handler(buff, sfd);
                    }
                }
            }
        }
    }
    close(sfd);
}
// handl user registration i.e PASS pass / USER username :realname / NICK nickname

bool Client::client_reg(std::string cmd, std::string param){

}

bool server::cmd_handler(char *buff, int sfd){
    std::string cmd = buff;
    if (cmd == "NICK" || cmd == "PASS" || cmd == "USER")
        client_reg(cmd, sfd);

}

void server::set_sfd(int fd){
    sfd = fd;
}

server::server(const char *port, const char *password){
    _port = port;
    _password = password;
}

int main(int argc, char const *argv[])
{
    if (argc > 2){
        server sever(argv[1], argv[2]);
        sever.server_setup();
    }
    return 0;
}
                          