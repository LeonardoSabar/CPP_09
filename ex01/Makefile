NAME		:= RPN

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -Iinclude

SRC_DIR		:= src
OBJ_DIR		:= build
INC_DIR		:= include

SRC			:= $(SRC_DIR)/main.cpp $(SRC_DIR)/RPN.cpp
OBJ			:= $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

#Colors:
GREEN		:= \e[92;5;118m
YELLOW		:= \e[93;5;226m
GRAY		:= \e[33;2;37m
RESET		:= \e[0m
CURSIVE		:= \e[33;3m

all: $(NAME)

$(NAME): $(OBJ)
	@printf "$(CURSIVE)$(GRAY)- Compiling $(NAME)... $(RESET)\n"
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)- Executable ready.\n$(RESET)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@printf "$(CURSIVE)$(GRAY)- Compiling $<... $(RESET)\n"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "[OK] Compilado: $< -> $@"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Objects files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "Executable file removed."

re: fclean all

.PHONY: all clean re