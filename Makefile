# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 14:15:25 by jdufour           #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2024/01/24 19:15:35 by juandrie         ###   ########.fr        #
=======
#    Updated: 2024/01/24 12:04:28 by juandrie         ###   ########.fr        #
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
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
	srcs/builtins/ft_export.c \
	srcs/builtins/export_utils.c \
	srcs/builtins/export_utils2.c \
	srcs/builtins/pwd.c \
	srcs/builtins/unset.c \
	srcs/builtins/signals.c \
	srcs/exec/pipe.c \
	srcs/exec/pipe2.c \
	srcs/exec/pipe3.c \
	srcs/exec/pipe4.c \
	srcs/exec/exec.c \
	srcs/exec/exec2.c \
	srcs/exec/heredoc.c \
	srcs/exec/heredoc2.c \
	srcs/exec/path.c \
	srcs/exec/redirection.c \
	srcs/exec/redirection2.c \
	srcs/exec/redir_infile.c \
	srcs/exec/redir_outfile.c \
	srcs/exec/redir_outfile2.c \
	srcs/lexer/lexer.c \
	srcs/parser/expand.c \
	srcs/parser/init_parser.c \
	srcs/parser/types.c \
	srcs/parser/tokens.c \
	srcs/utils/init_structs.c \
	srcs/utils/init_structs2.c \
	srcs/utils/heredoc_utils.c \
	srcs/utils/toolbox_utils.c \
	srcs/utils/redirect_utils.c \
	srcs/utils/cd_utils.c \
	srcs/utils/allocs/allocs.c \
	srcs/utils/allocs/frees.c \
	srcs/utils/allocs/struct.c \
	srcs/utils/allocs/joins.c \

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

run: $(NAME)
	@echo "$(GREEN)Lancement de $(NAME) dans une nouvelle fenêtre de terminal...$(NC)"
	@xterm -fa 'Monospace' -fs 12 -e './$(NAME)' &

.PHONY: all clean fclean re


