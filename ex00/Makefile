NAME		:= btc

CC			:= c++
FLAGS		:= -Wall -Wextra -Werror -std=c++98 -g3

ifdef DEBUG
FLAGS		+= -DDEBUG
endif

HEADER		:= $(shell find . -name "*.hpp") $(shell find . -name "*.tpp")
SRC			:= $(shell find . -name "*.cpp")
OBJ			:= $(SRC:%.cpp=build/%.o)

#Colors:
GREEN		:= \e[92;5;118m
YELLOW		:= \e[93;5;226m
GRAY		:= \e[33;2;37m
RESET		:= \e[0m
CURSIVE		:= \e[33;3m

all: $(NAME)

$(NAME): build $(OBJ)
	@printf "$(CURSIVE)$(GRAY)- Compiling $(NAME)... $(RESET)\n"
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)- Executable ready.\n$(RESET)"

build/%.o: %.cpp $(HEADER)
	@printf "$(CURSIVE)$(GRAY)- Compiling $<... $(RESET)\n"
	@$(CC) $(FLAGS) -c $< -o $@

build:
	@mkdir -p build

clean:
	@rm -rf build
	@rm -rf $(NAME)
	@printf "$(YELLOW)- Executable removed.$(RESET)\n"

fclean: clean

re: fclean all

.PHONY: all clean re
