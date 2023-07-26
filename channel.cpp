#include"channel.hpp"

channel::channel(/* args */)
{
}

channel::channel(const channel &obj)
{
    this->chan_members = obj.chan_members;
    this->clients_fd = obj.clients_fd;
    this->secure = obj.secure;
    this->_operators_fd = obj._operators_fd;
    this->_chan_name  =obj._chan_name;
    this->_cl_fd = obj._cl_fd;
}

// bool channel::is_secure(std::string chan_name){
//     if (chan_map.at(chan_name).secure)
// }


void    channel::add_member(int c_fd, std::string name){
    this->clients_fd.push_back(c_fd);
    this->chan_members.insert(std::pair<std::string,std::vector<int> >(name, this->clients_fd));
}

// add channel to channel map and setname and member as an operator
bool channel::add_channel(std::vector<std::string> vec,Client &cl){
    if (vec.size() < 3){
        this->_chan_name = vec[1];
        this->secure = false;
        this->clients_fd.push_back(cl.get_clientfd());
        this->chan_members.insert(std::pair<std::string,std::vector<int> >(vec[1], this->clients_fd));
        //handle user operator and send all mssg and list cmds to execute
        this->_operators_fd.push_back(cl.get_clientfd());
        return true;
    }
    return false;
}



channel::~channel()
{
}
