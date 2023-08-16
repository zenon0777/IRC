CC=c++
CFLAGS= -Werror -Wall -Wextra  -std=c++98
SRCS= 	auth.cpp channel.cpp chatgpt.cpp client.cpp engrafiete_sto_kanali.cpp invite_sto_kanali.cpp\
		kick.cpp modes.cpp nickname.cpp privmssg.cpp\
		ser.cpp server_setup.cpp topic.cpp user.cpp utils.cpp

INC= client.hpp ser.hpp channel.hpp

OBJ =$(subst .cpp,.o,$(SRCS))

%.o: %.cpp $(INC)
	$(CC) -c -o $@ $< $(CFLAGS)
all: ircserv

ircserv: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) -lcurl
clean:
	rm -rf $(OBJ)
fclean : clean
	rm -rf ircserv
re:fclean
	$(MAKE)
