CC = c++
# FLAGS = -std=c++98 -Werror -Wall -Wextra
FLAGS = -g -std=c++98
NAME = webserv

SRC = src/main.cpp src/Config.cpp src/Operator.cpp src/Server.cpp src/Poller.cpp \
    src/Client.cpp src/Socket.cpp src/RequestParser.cpp src/Handler.cpp src/Response.cpp \
    src/Cgi.cpp

HEADERS := incl/Operator.hpp incl/Server.hpp incl/Poller.hpp incl/Client.hpp \
    incl/Socket.hpp incl/Handler.hpp incl/Cgi.hpp

GREEN := \033[0;32m
YELLOW := \033[1;33m

all: $(NAME)

$(NAME): $(SRC) $(HEADERS)
	@$(CC) $(FLAGS) $(SRC) -o $(NAME)
	@echo "\n$(GREEN)$(NAME) created$(DEFAULT)"

clean:
	@rm -f $(NAME)
	@echo "\n$(YELLOW)All cleaned!$(DEFAULT)"

re: clean all