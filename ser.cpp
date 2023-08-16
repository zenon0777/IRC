#include "ser.hpp"

// handl user registration i.e PASS pass / USER username :realname / NICK nickname

bool server::command_parse(std::vector<std::string> vec, int client_fd)
{
    bool auth = cl.at(client_fd).get_authent();
    if (!vec.empty() && vec[0] == "PASS")
    {
        if (vec.size() < 2)
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 461 " + cl.at(client_fd).get_nickname();
            err += " :Not enough parameters\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        int flag = authenticateClient(vec, client_fd);
        if (flag == 1)
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 462 " + cl.at(client_fd).get_nickname();
            err += " :You may not reregister\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
            return false;
        }
        else if (flag == 2)
        {
            std::string err = ":" + cl.at(client_fd).get_host() + " 464 " + cl.at(client_fd).get_nickname();
            err += " :password incorrect\r\n";
            const char *buff = err.c_str();
            send(client_fd, buff, strlen(buff), 0);
        }
    }
    else if (vec.size() >= 1)
    {
        if (vec[0] == "NICK" && auth == true)
        {
            if (nickname_cmd(vec, client_fd) ==  false)
                return false;
        }
        else if (vec[0] == "USER" && auth == true)
        {
            if (user_cmd(vec, client_fd) == false)
                return false;
        }
        else if (vec[0] == "PRIVMSG" && auth == true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * PRIVMSG :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            handle_recievers(vec, client_fd);    
        }    
        else if (vec[0] == "JOIN" && auth== true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * JOIN :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            if (engrafiete_sto_kanali(vec, client_fd) == false)
                return false;
        }
        else if (vec[0] == "/chat" && auth== true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * JOIN :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            if (bot(vec, client_fd) == false)
                return false;
            // parse_response(vec[0], client_fd);
        }
        else if (vec[0] == "INVITE" && auth == true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * INVITE :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            invite_sto_kanali(vec, client_fd);
        }
        else if (vec[0] == "KICK" && auth ==  true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * KICK :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            if (kick_memeber(vec, client_fd) == false)
                return false;
        }
        else if (vec[0] == "TOPIC" && auth == true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * TOPIC :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            topicate_channel(vec, client_fd);
        }
        if (vec[0] == "MODE" && auth == true)
        {
            if (cl[client_fd].is_registred < 2)
            {
                //:punch.wa.us.dal.net 451 * join :You must finish connecting with another nickname first.
                std::string notif = ":" + cl[client_fd].get_host() + " 451 * MODE :You have not registered\r\n";
                const char *rpl = notif.c_str();
                send(client_fd, rpl, strlen(rpl), 0);
                return false;
            }
            mode_change(vec, client_fd);
        }
        else if (auth == false)
        {
            send(client_fd, "Need authentification to continue:...\r\n", 40, 0);
            return false;
        }
    }
    return true;
}


bool server::cmd_handler(char *buff, int client_fd)
{
    std::string cmd(buff);
    cmd = trim(cmd, " \t\n\r");
    std::vector<std::string> vec;
    std::vector<std::string> cmd_1;
    vec = splite(cmd, ' ');
    std::cout << cmd << std::endl;
    if (cmd.find_last_of('\r') != std::string::npos)
    {
        cmd_1 = splite(cmd, '\n');
        for (size_t i =0; i < cmd_1.size(); i++)
        {
            cmd = cmd_1[i];
            std::cout << cmd << "   ME " << std::endl;
            vec = splite(cmd, ' ');
            if (command_parse(vec, client_fd) == false)
                return false;
        }
    }
    else
    {
        if (command_parse(vec, client_fd) == false)
            return false;
    }
    return true;
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
