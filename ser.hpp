#ifndef SER_HPP

#define SER_HPP
#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <unistd.h>
#include <map>
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
#include <curl/curl.h>
#include "channel.hpp"

#define g_max_clients 3
const std::string API_KEY = "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";
const std::string API_URL = "https://api.openai.com/v1/chat/completions";
class server
{
private:
    std::vector<struct pollfd> pfds;
    const char *_port;
    int sfd;
public:
    // Client clients[g_max_clients];
    std::string _password;
    Client clients;
    channel *channels;
    std::map<std::string, channel *>chan_map;
    std::map<int , Client> cl;
    server(const char *port, const char *password);
    server();
    int server_socket();
    bool command_parse(std::vector<std::string> vec, int client_fd);
    int authenticateClient(std::vector<std::string> c, int listener);
    bool nickname_cmd(std::vector<std::string> &vec, int c_fd);
    bool kick_memeber(std::vector<std::string> vec, int client_fd);
    bool topicate_channel(std::vector<std::string> vec, int client_fd);
    bool bot(std::vector<std::string> vec, int client_fd);
    void add_opers(std::vector<std::string> vec, int client_fd);
    void add_reply(std::string, int cfd, std::string mssg);
    void limite_reply(std::string name, int oper, std::string);
    void take_reply(std::string name, int cfd, std::string mssg);
    bool take_mode(int cfd, std::vector<std::string> vec);
    bool add_mode(int cfd, std::vector<std::string> vec);
    bool take_opers(std::vector<std::string> vec, int client_fd);
    bool take_pass(std::vector<std::string> vec, int client_fd);
    void oper_rply(std::string, int, int, std::string);
    bool valid_nick(int cfd, std::string str);
    void key_reply(std::string name, int oper, std::string);
    bool change_topic(int, std::vector<std::string>);
    bool user_cmd(std::vector<std::string>vec, int c_fd);
    void mode_change(std::vector<std::string> vec, int client_fd);
    void kick_rply(std::string name, int oper, int cfd);
    std::vector<std::string> splite(std::string str, char delim);
    bool is_identical(std::string nick, int cfd);
    bool is_channelexist(std::string name);
    void removeclients(int cfd, int cfd2);
    bool handle_recievers(std::vector<std::string> vec, int c_fd);
    bool send_messg(std::string mssg, int, int client_fd, std::string);
    int server_setup();
    bool is_operator(std::string chan_name, int client_fd);
    channel get_channel(std::string);
    int get_clientfd(std::string name);
    bool invite_sto_kanali(std::vector<std::string> vec, int client_fd);
    bool engrafiete_sto_kanali(std::vector<std::string> vec, int client_fd);
    bool client_exist(std::string name, int client_fd);
    int get_port();
    void sendWelcomeMessage(int clientSocket, const std::string& nickname, const std::string& username);
    void set_sfd(int sfd);
    bool reply(std::string name, int, bool flag);
    bool clinet_invited(std::string name, int client_fd);
    std::string get_pass();
    bool cmd_handler(char *mssg, int client_fd);
    void parse_response(std::string response, int);
    // void sendfile(const char * ippadress, char *file);
    ~server();
};


std::string trim(const std::string &str, std::string set);
std::vector<std::string> splite(std::string str, char delim);
#endif