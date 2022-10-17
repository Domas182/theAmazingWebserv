# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sreinhol <sreinhol@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/28 20:06:19 by sreinhol          #+#    #+#              #
#    Updated: 2022/10/17 21:59:06 by sreinhol         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
FLAGS = -std=c++98 -Wall -Werror -Wextra
# FLAGS = -g -std=c++98
NAME = webserv

SRC = src/main.cpp src/Config.cpp src/Operator.cpp src/Server.cpp src/Poller.cpp \
	src/Client.cpp src/Socket.cpp
HEADERS := src/Operator.hpp src/Server.hpp src/Poller.hpp src/Client.hpp \
	src/Socket.hpp

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