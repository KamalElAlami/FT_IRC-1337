CPP			=	c++
NAME			=	ircserv
SRC			=	main.cpp srcs/Server.cpp srcs/Client.cpp srcs/AuthenCmd.cpp \
				srcs/handleConnections.cpp srcs/Channels.cpp srcs/ChannelsCmds.cpp  srcs/Utils.cpp  srcs/chat.cpp\
				srcs/commands/Cap.cpp srcs/commands/Nick.cpp srcs/commands/Part.cpp srcs/commands/Pass.cpp\
				srcs/commands/Ping.cpp srcs/commands/Privmsg.cpp srcs/commands/User.cpp
FLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address
OBJ			=	$(SRC:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJ)
			$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o			:	%.cpp ./includes/Server.hpp ./includes/Client.hpp ./includes/Channels.hpp ./includes/chat.hpp
			$(CPP) $(FLAGS) -c $< -o $@

clean		:
			rm -rf $(OBJ)

fclean		:	clean
			rm -rf $(NAME)

re			:	fclean all