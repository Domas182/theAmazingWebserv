CC = c++

FLAGS = -g -std=c++98 -Wall -Wextra -Werror

NAME = webserv

SRC_DIR = ./src
SRC	=	$(shell find $(SRC_DIR) -name "*.cpp" -execdir echo {} ";")

OBJ_DIR	=	./obj
OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

DEPS	=	$(OBJS:.o=.d)

all: $(NAME)

-include $(DEPS)

HEADERS := incl/Operator.hpp incl/Server.hpp incl/Poller.hpp incl/Client.hpp \
    incl/Socket.hpp incl/Handler.hpp incl/Cgi.hpp

GREEN := \033[0;32m
YELLOW := \033[1;33m

$(NAME): $(OBJ) $(HEADERS)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo "\n$(GREEN)$(NAME) created$(DEFAULT)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | prep
	@$(CC) $(FLAGS) -MMD -MP -c $< -o $@

prep:
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJS) $(OBJ_DIR)
	@echo "\n$(YELLOW)All cleaned!$(DEFAULT)"

fclean: clean
	@rm -f $(NAME)
	@echo "\n$(YELLOW)and remade!$(DEFAULT)"

re: fclean all