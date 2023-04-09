CC = g++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = $(wildcard ./src/*.cpp ./src/commands/*.cpp)

OBJ = $(SRC:.cpp=.o)

NAME = ft_irc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@rm -rf $(OBJ)
clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
