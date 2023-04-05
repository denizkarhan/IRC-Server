CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = $(shell find ./src -name "*.cpp")

OBJ = $(SRC:.cpp=.o)

NAME = ft_irc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
