#include"channel.hpp"

channel::channel()
{
}

channel::channel(const channel &obj)
{
    this->chan_members  = obj.chan_members;
    this->clients_fd    = obj.clients_fd;
    this->chan_password = obj.chan_password;
    this->secure        = obj.secure;
    this->user_limite   = obj.user_limite;
    this->is_limited    = obj.is_limited;
    this->nbr_member    = obj.nbr_member;
    this->is_inviteonly = obj.is_inviteonly;
    this->is_topic      = obj.is_topic;
    this->topic         = obj.topic;
    this->is_topicated  = obj.is_topicated;
    this->_operators_fd = obj._operators_fd;
    this->_chan_name    = obj._chan_name;
}

// bool channel::is_secure(std::string chan_name){
//     if (chan_map.at(chan_name).secure)
// }

bool channel::is_member(int c_fd, std::string chan_name){
    std::map<std::string, std::vector<int> >::iterator mit;
    std::vector<int>::const_iterator vit;
    for (mit = this->chan_members.begin(); mit != this->chan_members.end(); ++mit){
        if (mit->first == chan_name)
        {
            for (vit = mit->second.begin(); vit != mit->second.end(); ++vit)
            {
                if (*vit == c_fd)
                    return true;
            }
        }
    }
    return false;
}

/*:lop!~zen@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#there
:punch.wa.us.dal.net 353 lop = #there :@lop 
:punch.wa.us.dal.net 366 lop #there :End of /NAMES list.
:lk!~zeno@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#there

*/

std::vector<int> channel::get_chan_member()
{
    return this->clients_fd;
}

void    channel::add_member(Client &cl, std::string name){
    std::map<std::string, std::vector<int> >::iterator mit;
    std::vector<int>::const_iterator vit;
    if (is_member(cl.get_clientfd(), name) == true)
        return ;
    for (mit = this->chan_members.begin(); mit != this->chan_members.end(); ++mit){
        if (mit->first == name)
            mit->second.push_back(cl.get_clientfd());
    }
    clients_fd.push_back(cl.get_clientfd());
    this->user_nickname[cl.get_clientfd()] = cl.get_nickname();
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

void channel::remove_member(Client &cl, std::string name)
{
    std::map<std::string, std::vector<int> >::iterator mit;
    std::vector<int>::const_iterator vit;
    for (mit = this->chan_members.begin(); mit != this->chan_members.end(); ++mit){
        if (mit->first == name)
        {
            for (vit = mit->second.begin(); vit != mit->second.end(); ++vit)
            {
                if (*vit == cl.get_clientfd())
                    mit->second.erase(vit);
            }
        }
    }
    this->user_nickname.erase(cl.get_clientfd());
}


bool channel::add_mode(int cfd, std::vector<std::string> vec){
    if (this->is_operator(vec[1], cfd) == true)
    {
        std::string s = vec[2];
        for (int i =1; i < s.length(); i++)
        {
            if (s[i] == 'i')
                this->is_inviteonly = true;
            if (s[i] == 't')
            {
                this->is_topicated = true;
            }
            else
                return false;
        }
    }
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
/*join #there
:lop!~l@5c8c-aff4-7127-3c3-1c20.230.197.ip JOIN :#there
:punch.wa.us.dal.net 353 lop = #there :@lop 
:punch.wa.us.dal.net 366 lop #there :End of /NAMES list.
*/
bool channel::add_channel(std::string chan_name,Client &cl, bool secure)
{
    if (secure == false)
    {
        this->_chan_name = chan_name;
        this->secure = false;
        this->is_inviteonly = false;
        this->is_topic = true;
        this->is_topicated = true;
        this->topic = "";
        this->is_limited = false;
        this->user_limite = 0;
        this->nbr_member = 0;
        this->clients_fd = {0};
        this->user_nickname[cl.get_clientfd()] = cl.get_nickname();
        this->chan_members.insert(std::pair<std::string,std::vector<int> >(chan_name, this->clients_fd));
        //handle user operator and send all mssg and list cmds to execute
        this->_operators_fd.push_back(cl.get_clientfd());
        //print 
        std::cout << this->_operators_fd[0] << std::endl;
        if (this->_operators_fd.size() == 2)
            std::cout << this->_operators_fd[1] << std::endl;
        std::cout << cl.get_clientip() << std::endl;
        return true;
    }
    return false;
}



channel::~channel()
{
}
