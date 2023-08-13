#include"ser.hpp"

bool server::user_cmd(std::vector<std::string>vec, int c_fd){
    std::string r_name;
    if (vec.size() < 4)
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 461 " + cl.at(c_fd).get_nickname();
        err += " :Not enough parameters\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
        return false;
    }
    // :punch.wa.us.dal.net 462 los :You may not reregister
    else if (cl[c_fd].is_registred >= 2)
    {
        std::string err = ":" + cl.at(c_fd).get_host() + " 462 " + cl.at(c_fd).get_nickname();
        err += " :You may not reregister\r\n";
        const char *buff = err.c_str();
        send(c_fd, buff, strlen(buff), 0);
        return false;
    }
    else
    {
        if (*vec[3].begin() == ':')
            r_name = vec[3].substr(1);
        else
            r_name = vec[3];
        cl.at(c_fd).set_username(vec[1], r_name);
        cl[c_fd].is_registred += 1;
        return true;
    }
    return false;
}
