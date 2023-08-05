#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include<map>
#include<string>
#include<vector>
#include"client.hpp"
class channel
{
private:
public:
    bool secure;
    bool is_inviteonly;
    int  user_limite;
    bool is_limited;
    size_t nbr_member;
    std::string chan_password;
    std::string topic;
    bool is_topic;
    bool is_topicated;
    std::string _chan_name;
    std::vector<int>clients_fd;
    std::map<std::string, std::vector<int> > chan_members;
    std::map<int ,std::string> user_nickname;
    std::vector<int>_operators_fd;
    std::map<std::string,std::vector<int> > get_chan_member();
    void add_member(Client &, std::string);
    void remove_member(Client &, std::string);
    bool add_mode(int cfd, std::vector<std::string> vec);
    bool is_secure(std::string chan_name);
    void set_topicated(bool);
    std::string get_chan_name() const;
    void set_topic(std::string);
    std::string get_topic(void) const;
    bool get_topicated(void) const;
    std::vector<int> get_operators(void) const;
    bool is_member(int c_fd, std::string chan_name);
    bool change_topic(int, std::vector<std::string>);
    bool is_operator(std::string chan_name, int client_fd);
    bool add_channel(std::string chan_name,Client &cl, bool secure);
    bool add_client(std::vector<std::string>,Client &cl);
    channel();
    channel(const channel &obj);
    channel(int id, std::string cah_name, Client cl);
    ~channel();
};



#endif