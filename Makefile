CC = g++

FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = $(wildcard ./src/*.cpp ./src/commands/*.cpp)

OBJ_DIR = ./build
OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo ircServer created

$(OBJ_DIR)/%.o: ./src/commands/%.cpp
	@$(CC) $(FLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: ./src/%.cpp
	@$(CC) $(FLAGS) -c -o $@ $<

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
