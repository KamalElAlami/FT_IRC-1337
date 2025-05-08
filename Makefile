CPP			=	c++
NAME		=	ircserv
SRC			=	main.cpp Srcs/Server.cpp Srcs/Client.cpp
FLAGS		=	-Wall -Wextra -Werror -std=c++98  
OBJ			=	$(SRC:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJ)
			$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o			:	%.cpp ./Includes/Server.hpp ./Includes/Client.hpp
			$(CPP) $(FLAGS) -c $< -o $@

clean		:
			rm -rf $(OBJ)

fclean		:	clean
			rm -rf $(NAME)

re			:	fclean all