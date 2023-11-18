/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:46:13 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/18 23:21:21 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// System Libraries

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/syscall.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "libft/libft.h"

enum type {
	WORD = 1,
	PIPE,
	LEFT_CHEV,
	RIGHT_CHEV,
	DB_LEFT_CHEV,
	DB_RIGHT_CHEV,
	EXPAND
};

# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34

typedef struct s_quotes {
	int		single_quotes;
	int		double_quotes;
	int		single_embedded;
	int		double_embedded;
	int		single_pairs;
	int		double_pairs;
	bool	is_quotes;
}	t_quotes;

typedef struct s_parsed {
	char			*command;
	int				type;
}	t_parsed;

typedef struct s_alloc {
	void	**adr;
	int		count;
}	t_alloc;

//Lexer

int		is_quote(char c);
int		error_quotes(t_quotes *quotes);
void	embedded_quotes(char *line, t_quotes *quotes);
void	quotes_count(char *line, t_quotes *quotes);

//Parser



//Utils

void	add_to_garbage(void *ptr, t_alloc *garbage);
void	free_garbage(t_alloc *garbage);

#endif