#compiler
NAME = btc
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

# directories
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = bin

# archives
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/BitcoinExchange.cpp
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo "$(NAME) it's Done!"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	echo "Objects files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "Executable file removed."

re: fclean all

.PHONY: all clean fclean re
