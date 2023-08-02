#include"channel.hpp"

channel::channel(/* args */)
{
}

channel::channel(const channel &obj)
{
    this->chan_members = obj.chan_members;
    this->is_topic = obj.is_topic;
    this->topic = obj.topic;
    this->clients_fd = obj.clients_fd;
    this->secure = obj.secure;
    this->_operators_fd = obj._operators_fd;
    this->_chan_name  =obj._chan_name;
    this->_cl_fd = obj._cl_fd;
}

// bool channel::is_secure(std::string chan_name){
//     if (chan_map.at(chan_name).secure)
// }

bool channel::is_member(int c_fd, std::string chan_name){
    std::map<std::string, std::vector<int> >::iterator mit;
    std::vector<int>::const_iterator vit;
    for (mit = this->chan_members.begin(); mit != this->chan_members.end(); ++mit){
        std::cout << mit->first << " :::  "<<std::endl;
        if (mit->first == chan_name)
        {
            for (vit = mit->second.begin(); vit != mit->second.end(); ++vit)
            {
                std::cout << *vit << "::: \n";
                if (*vit == c_fd)
                    return true;
            }
        }
    }
    return false;
}

void    channel::add_member(int c_fd, std::string name){
    std::map<std::string, std::vector<int> >::iterator mit;
    std::vector<int>::const_iterator vit;
    for (mit = this->chan_members.begin(); mit != this->chan_members.end(); ++mit){
        std::cout << mit->first << " :::  "<<std::endl;
        if (mit->first == name)
        {
            mit->second.push_back(c_fd);
            std::cout << " ::2:: " << mit->second[0] <<std::endl;
        }
    }
    // add member channel obj
}

bool channel::is_operator(std::string chan_name, int client_fd){
    std::vector<int>::const_iterator it;
    for(it=this->_operators_fd.begin(); it!= this->_operators_fd.end(); ++it){
        if (*it == client_fd)
            return true;
    }
    return false;
}

std::string channel::get_chan_name() const{
    return this->_chan_name;
}

bool channel::add_mode(int cfd, std::vector<std::string> vec){
    if (this->is_operator(vec[1], cfd) == true){
        size_t pos = vec[2].find('+');
        std::string s = vec[2].substr(pos);
        // if (s == "o")
        //     user_to_operator()
        if (s == "k")
        {
            this->secure = true;
            this->chan_password = vec[2];
        }
        for (int i =0; i < s.length(); i++)
        {
            if (s[i] == 'i')
                this->is_inviteonly = true;
            
        }
    }
    return true;
}

bool channel::change_topic(int cfd, std::vector<std::string> vec){
    const char *buff;
    // if (this->is_operator(vec[1], cfd) == true)
    // {
        if (vec.size() == 2 && this->is_topic == false)
            send(cfd, "NO topic set Yet : you can set it as u r an operator\n", 54, 0);
        else if (vec.size() == 2 && this->is_topic == true)
        {
            buff = this->topic.c_str();
            send(cfd, buff, sizeof(buff), 0);
        }
        else if (vec.size() == 3){
            this->is_topic = true;
            this->topic = vec[2];
        }
        else if (vec.size() == 1)
            send(cfd, "ERR_Parameter : ...\n",20, 0);
    // }
    return true;
}

std::vector<int> channel::get_operators(void) const{
    return this->_operators_fd;
}

std::string channel::get_topic(void) const{
    return this->topic;
}

bool channel::get_topicated(void) const{
    return this->is_topic;
}

void channel::set_topicated(bool flag){
    this->is_topic = flag;
}

void channel::set_topic(std::string str){
    this->topic = str;
}

// add channel to channel map and setname and member as an operator
bool channel::add_channel(std::vector<std::string> vec,Client &cl){
    if (vec.size() < 3){
        this->_chan_name = vec[1];
        this->secure = false;
        this->is_topic = false;
        this->topic = "";
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
