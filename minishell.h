/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:46:13 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 18:54:07 by jdufour          ###   ########.fr       */
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
# include <math.h>

enum e_type {
	WORD = 1,
	PIPE,
	LEFT_CHEV,
	RIGHT_CHEV,
	DB_LEFT_CHEV,
	DB_RIGHT_CHEV,
	EXPAND,
};

# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define UNDERSCORE 95
# define REDIRECT_OUTPUT 1
# define REDIRECT_INPUT 2
# define NO_REDIRECTION 3
# define REDIRECT_APPEND_OUTPUT 4
# define REDIRECT_APPEND_INPUT 5

typedef struct	s_quotes {
	bool	case_single;
	bool	case_double;
}	t_quotes;

typedef struct	s_expand {
	int		pos;
	int		lex_index;
}	t_expand;

typedef struct	s_command {
	char			*word;
	int				type;
	bool			is_expand;
}	t_command;

typedef struct	s_alloc {
	void	**adr;
	int		count;
}	t_alloc;


typedef struct	s_exec {
	char	*command;
	char	*file;
	int		redirect_type;
	char	*redirection_file;
	int		redirection_type;
	int		redirection_append;
}	t_exec;

typedef struct	s_pipe {
	char	*command1;
	char	*command2;
	int		pipefd[2];
	int		dup_fd;
}	t_pipe;

typedef struct	s_code {
	int	code_status;
}	t_code;

typedef struct s_line {
	char			*line;
	struct s_line	*next;
}	t_line;


//Lexer
int			is_in_quote(char c, t_quotes *quotes);
void		error_quotes(char *line, t_quotes *quotes);
void		ft_error_lexer(t_command *command);
int			special_types(char c);
void		get_type(t_command *token);
int			is_expand(char *line);
char		*get_env_var_name(t_command *lexer, t_expand *expand, char *name, t_quotes *quotes);

//Parser
char	**parse_command_line(char *input);
void	free_parsed_command_line(char **argv);
int		 expand_size(char *var);

//Utils
t_command	*ft_struct_join(t_command *tok1, t_command tok2);
void		ft_free_command(t_command *command);
char		*char_to_str(char c);

//Execve
char	*find_command_in_segment(char *segment, char *command);
char	*find_command_path(char *command);
void	execute_command(char *input, char **envp);
void	handle_command(char *input, t_code *code, char **argv, char **envp);

//Redirection 
void	redir(t_exec *exec, char **argv, char **envp);
void	here_doc(const char *delimiter, t_pipe *pipes, char **argv, char **envp);
int		handle_redirection(t_exec *exec, char *input, char **argv, char **envp);

//Pipe
void	execute_pipe(t_pipe *pipes, char **argv1, char **argv2, char **envp);
void	process_pipe(char *command, t_pipe *pipes, char **argv, char **envp);

//Builtins
int		my_cd(char **args);
int		my_echo(char **argv);
int		my_env(char **envp);
int		my_exit(char **cmd_args);
int		my_export(char **envp);
int		my_pwd(char **unused_args, char **unused_envp);
int		my_unset(char ***envp, char **names);
void	execute_status_builtin(t_code *code);

#endif