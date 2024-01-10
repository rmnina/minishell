/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:03:22 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/10 16:41:02 by juandrie         ###   ########.fr       */
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
# include <sys/param.h>
# include <dirent.h>
# include <errno.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "../libft/libft.h"
# include <math.h>
# include <signal.h>
# include <errno.h>

enum e_type {
	WORD = 1,
	PIPE,
	LEFT_CHEV,
	RIGHT_CHEV,
	DB_RIGHT_CHEV,
	DB_LEFT_CHEV,
	EXPAND,
	CODE,
};

# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define UNDERSCORE 95
# define SPECIAL_EXIT_CODE 255


typedef struct s_quotes {
	bool	case_single;
	bool	case_double;
	bool	case_quotes;
	int		vpos;
	int		npos;
	char	*var;
}	t_quotes;

typedef struct s_command {
	char			*word;
	int				type;
}	t_command;

typedef struct s_pipe {
	char	**command;
	int		fd[2];
	pid_t	pid;
	int		heredoc_fd[2];
}	t_pipe;

typedef struct s_code {
	int		code_status;
	char	*current;
	char	*last;
}	t_code;

typedef struct s_line {
	char			*line;
	struct s_line	*next;
}	t_line;

typedef struct s_heredocNode {
	char					*delimiter;
	struct s_heredocNode	*next;
}	t_heredocNode;


//Lexer
int			is_in_quote(char c, t_quotes *quotes);
int			error_quotes(char *line, t_quotes *quotes);
int			ft_error_lexer(t_command *command);
int			special_types(char c1, char c2);
void		get_type(t_command *token, t_quotes *quotes);
int			is_expand(char *line);

//Parser
void		free_parsed_command_line(char **argv);
int			parse_quotes(char *line, int *i, t_quotes *quotes);
int			parse_expand_quotes(char *line, int *i, t_quotes *quotes);
char		*get_env_var_name(char *line, int *i, t_alloc **garbage);
void		init_get_token(t_command *token);
void		init_get_expand(t_command *token, char *line, int *i, t_quotes *quotes, t_alloc **garbage, char ***envp);
t_command	*get_command(char *line, t_quotes *quotes, t_alloc **garbage, char ***envp);
int			get_lex_expand(char *line, int *i, t_quotes *quotes, \
t_command *token, t_alloc **garbage, char ***envp);
t_command	*ft_parsing(char *line, t_alloc **garbage, char ***envp);
char		*ft_getenv(char ***envp, const char *name);

//Utils
t_command	*ft_struct_join(t_command *tok1, t_command tok2, t_alloc **garbage);
void		ft_free_command(t_command *command);
char		*char_to_str(char c, t_alloc **garbage);
int			special_type_expand(char c1, char c2);

//Execve
char		*find_command_in_segment(char *segment, char *command, t_alloc **garbage);
char		*find_command_path(char *command, t_alloc **garbage);
// void		execute_command(char **cmd_args, char **envp, t_alloc **garbage);
// void			handle_command(char *input, t_code *code, char **envp, t_alloc **garbage);
// int			execute_non_builtin(char **envp, t_code *code, char **cmd_args, t_alloc **garbage);
// void		heredoc_child(t_pipe *pipes, char **argv, char **envp, t_alloc **garbage);
void		execute_command(char **cmd_args, char ***envp, t_alloc **garbage);
void		handle_command(char *input, t_code *code, char ***envp, t_alloc **garbage, t_pipe *pipes);
int			execute_non_builtin(char ***envp, t_code *code, char **cmd_args, t_alloc **garbage);
void		heredoc_child(t_pipe *pipes, char **argv, char ***envp, t_code *code, t_alloc **garbage);
char		**create_cmd_args(t_command *command, int *i, t_alloc **garbage);
void		pick_command(char **cmd_args, char **envp, t_code *code, t_alloc **garbage);

//Redirection 
int			init_redirection(t_command *command, int *i, char **cmd_args, char ***envp, t_code *code, t_alloc **garbage);


//Pipe
pid_t		heredoc_pipe(t_pipe *pipes);
void		ft_multipipes(t_command *command, t_pipe *pipes, t_alloc **garbage, char ***envp, char **cmd_args, int *i, t_code *code);
//void	ft_multipipes(t_command *command, t_alloc *garbage, char ***envp, char **cmd_args, int *i, t_code *code);
//void execute_pipeline(t_command *command, int num_commands, char ***envp, t_code *code, t_alloc **garbage);


//Builtins
int			ft_cd(char **args, t_code *code);
int			ft_echo(char **argv, t_code *code);
int			ft_env(char **envp, t_code *code);
int			ft_exit(char **cmd_args, t_code *code, t_alloc **garbage);
int			ft_export(char ***envp, char **argv, t_code *code, t_alloc **garbage);
void		add_or_update_env_var(char ***envp, char *var, t_alloc **garbage);
int			ft_pwd(char **unused_args, char **unused_envp, t_code *code);
int			ft_unset(char ***envp, char **names, t_code *code);
int			execute_status_builtin(t_code *code, int *i);
int			execute_builtins(char **cmd_args, char ***envp, t_code *code, t_alloc **garbage);

//Signaux
void		child_handler(int signum);
int			process_prompt(void);
void		sigint_handler(int signum);
int			init_sigactionsa(struct sigaction *sa);
int			init_sigquit(void);
int			init_parent_signals(void);
//heredoc
int			heredoc(t_heredocNode *heredoclist, t_pipe *pipes, char **argv, char **envp, t_code *code, t_alloc **garbage);
void		read_add(int fd, const char *delimiter, t_alloc **garbage);

#endif