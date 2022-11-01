CC = c++
FLAGS = -std=c++98 -g -Wall -Wextra
# FLAGS = -g -std=c++98
NAME = webserv

SRC = src/main.cpp src/Config.cpp src/Operator.cpp src/Server.cpp src/Poller.cpp \
	src/Client.cpp src/Socket.cpp Request/RequestParser.cpp Handler/Handler.cpp Response/Response.cpp \
	src/Cgi.cpp

HEADERS := src/Operator.hpp src/Server.hpp src/Poller.hpp src/Client.hpp \
	src/Socket.hpp Handler/Handler.hpp src/Cgi.hpp

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