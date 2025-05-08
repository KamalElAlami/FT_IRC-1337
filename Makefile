CPP			=	c++
NAME		=	ircserv
SRC			=	main.cpp srcs/Server.cpp srcs/Client.cpp
FLAGS		=	-Wall -Wextra -Werror -std=c++98  
OBJ			=	$(SRC:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJ)
			$(CPP) $(FLAGS) $(OBJ) -o $(NAME)

%.o			:	%.cpp ./includes/Server.hpp ./includes/Client.hpp
			$(CPP) $(FLAGS) -c $< -o $@

clean		:
			rm -rf $(OBJ)

fclean		:	clean
			rm -rf $(NAME)

re			:	fclean all