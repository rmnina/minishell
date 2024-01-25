# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 14:15:25 by jdufour           #+#    #+#              #
#    Updated: 2024/01/25 16:17:06 by juandrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
TURQUOISE=\033[0;36m
PINK=\033[0;35m
NC=\033[0m

NAME = minishell

SRCS = srcs/main.c \
	srcs/builtins/cd.c \
	srcs/builtins/cd_utils.c \
	srcs/builtins/echo.c \
	srcs/builtins/env.c \
	srcs/builtins/exit.c \
	srcs/builtins/export/ft_export.c \
	srcs/builtins/export/export_utils.c \
	srcs/builtins/export/export_utils2.c \
	srcs/builtins/pwd.c \
	srcs/builtins/unset.c \
	srcs/builtins/signals.c \
	srcs/exec/heredoc/exec_heredoc.c \
	srcs/exec/heredoc/expand_heredoc.c \
	srcs/exec/pipe.c \
	srcs/exec/redirection/redir_infile.c \
	srcs/exec/redirection/redir_outfile.c \
	srcs/exec/redirection/redirection.c \
	srcs/exec/redirection/redirect_utils.c \
	srcs/exec/exec_process.c \
	srcs/exec/exec.c \
	srcs/exec/path.c \
	srcs/lexer/lexer.c \
	srcs/lexer/lexer2.c \
	srcs/parser/expand.c \
	srcs/parser/init_parser.c \
	srcs/parser/types.c \
	srcs/parser/tokens.c \
	srcs/parser/ft_parsing.c \
	srcs/utils/allocs/allocs.c \
	srcs/utils/allocs/frees.c\
	srcs/utils/allocs/joins.c\
	srcs/utils/allocs/structs.c\
	srcs/utils/init/init_structs.c \
	srcs/utils/init/init_structs2.c \
	srcs/utils/toolbox_utils.c \
	srcs/utils/toolbox_utils2.c \

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


