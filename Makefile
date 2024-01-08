# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 14:15:25 by jdufour           #+#    #+#              #
#    Updated: 2024/01/08 18:57:31 by juandrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
TURQUOISE=\033[0;36m
PINK=\033[0;35m
NC=\033[0m

NAME = minishell

SRCS = srcs/main.c \
	srcs/builtins/cd.c \
	srcs/builtins/echo.c \
	srcs/builtins/env.c \
	srcs/builtins/exit.c \
	srcs/builtins/export.c \
	srcs/builtins/pwd.c \
	srcs/builtins/unset.c \
	srcs/builtins/controlc.c \
	srcs/exec/exec_pipe.c \
	srcs/exec/exec.c \
	srcs/exec/heredoc.c \
	srcs/exec/path.c \
	srcs/exec/redirection.c \
	srcs/lexer/lexer.c \
	srcs/parser/expand.c \
	srcs/parser/init.c \
	srcs/parser/special_char.c \
	srcs/parser/tokens.c \
	srcs/utils/joins.c \

OBJS = $(SRCS:.c=.o)

CC = gcc

FLAGS = -g3 -ggdb -Wall -Wextra -Werror

RM = rm -rf

all: $(NAME)

$(NAME) : libft/libft.a $(OBJS)
	@echo "$(TURQUOISE)Compilation de $(NAME)...$(NC)"
	@$(CC) $(FLAGS) $(OBJS) -L libft -lft -lreadline -o $(NAME)

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

libft/libft.a:
	@echo "$(TURQUOISE)Compilation de la libft...$(NC)"
	@make -C libft/

clean:
	@echo "$(GREEN)Suppression des fichiers objets...$(NC)"
	@$(RM) $(OBJS)
	@make clean -C libft/

fclean: clean
	@echo "$(PINK)Suppression de $(NAME)...$(NC)"
	@$(RM) $(NAME)
	@echo "$(PINK)Suppression de la libft...$(NC)"
	@make fclean -C libft/

re: fclean all

.PHONY: all clean fclean re
