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
    std::string _chan_name;
    int _id;
    int _cl_fd;
    std::vector<int>_operators_fd;
    bool secure;
    std::vector<int>clients_fd;
    std::map<std::string, std::vector<int> > chan_members;
    std::map<std::string,std::vector<int> > get_chan_member();
    void add_member(int c_fd, std::string);
    bool is_secure(std::string chan_name);
    bool add_channel(std::vector<std::string>, Client&);
    bool add_client(std::vector<std::string>,Client &cl);
    channel();
    channel(const channel &obj);
    channel(int id, std::string cah_name, Client cl);
    ~channel();
};



#endif