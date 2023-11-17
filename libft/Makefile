# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/31 12:01:53 by jdufour           #+#    #+#              #
#    Updated: 2023/07/21 19:37:28 by jdufour          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_isalnum.c\
ft_isprint.c\
ft_memmove.c\
ft_putnbr_fd.c\
ft_strdup.c\
ft_strlcpy.c\
ft_strnstr.c\
ft_toupper.c\
ft_atoi.c\
ft_isalpha.c\
ft_memchr.c\
ft_memset.c\
ft_putstr_fd.c\
ft_striteri.c \
ft_strlen.c\
ft_strrchr.c\
ft_bzero.c\
ft_isascii.c\
ft_memcmp.c\
ft_putchar_fd.c\
ft_split.c\
ft_strjoin.c\
ft_strmapi.c\
ft_substr.c\
ft_calloc.c\
ft_isdigit.c\
ft_memcpy.c\
ft_putendl_fd.c\
ft_strchr.c\
ft_strlcat.c\
ft_strncmp.c\
ft_tolower.c\
ft_itoa.c\
ft_strtrim.c\
ft_printf.c\
ft_format.c\

SRCS_BONUS = ft_lstadd_front.c\
	    ft_lstadd_back.c\
	    ft_lstclear.c\
	    ft_lstdelone.c\
	    ft_lstiter.c\
	    ft_lstlast.c\
	    ft_lstnew.c\
	    ft_lstmap.c\
	    ft_lstsize.c\

OBJS = ${SRCS:.c=.o}

OBJS_BONUS = ${SRCS_BONUS:.c=.o}

HEADER = libft.h

CC = gcc

RM = rm -rfv

FLAGS = -Wall -Wextra -Werror

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${FLAGS} -c ${SRCS}
	ar cr ${NAME} ${OBJS}

bonus:  ${OBJS_BONUS}
	${CC} ${FLAGS} -c ${SRC_BONUS}
	ar cr ${NAME} ${OBJS_BONUS}

clean: 
	${RM} ${OBJS}
	${RM} ${OBJS_BONUS}
		
fclean: clean
	${RM} ${NAME}

re: fclean all
