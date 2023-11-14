# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/14 14:15:25 by jdufour           #+#    #+#              #
#    Updated: 2023/11/14 14:22:12 by jdufour          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =

OBJS = $(SCRS:%.c=%.o)

CC = gcc

FLAGS = -Wall -Wextra -Werror

INCLUDES = 

LIB = -lreadline -lft

RM = rm -rfv

all: $(NAME)

%.o : %.c
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(NAME) : $(OBJS)
	# make -C libft
	$(CC) $(FLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIB)

clean:
	$(RM) $(OBJS)
	# make -C libft clean

fclean: clean
		$(RM) $(NAME)
		# make -C libft fclean

re: fclean all

.PHONY: all clean fclean re