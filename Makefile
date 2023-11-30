# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 14:15:25 by jdufour           #+#    #+#              #
#    Updated: 2023/11/30 15:09:58 by jdufour          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
TURQUOISE=\033[0;36m
PINK=\033[0;35m
NC=\033[0m

NAME = minishell

SRCS = main.c \
	builtins/cd.c \
	builtins/code.c \
	builtins/echo.c \
	builtins/env.c \
	builtins/exit.c \
	builtins/export.c \
	builtins/pwd.c \
	builtins/unset.c \
	exec/exec_pipe.c \
	exec/exec.c \
	exec/heredoc.c \
	exec/path.c \
	exec/pipe_command.c \
	exec/redirection.c \
	lexer/error_lexer.c \
	lexer/expand.c \
	lexer/init.c \
	lexer/special_char.c \
	lexer/tokens.c \
	parser/parse_command.c \
	utils/frees.c \
	utils/joins.c \

OBJS = $(SRCS:.c=.o)

CC = gcc

FLAGS = -Wall -Wextra -Werror

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