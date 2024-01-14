/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:03:22 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 06:18:20 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ******************************* LIBRARIES ******************************* */

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

/* ******************************* MACROS ******************************* */

enum e_type {
	WORD = 1,
	CODE,
	PIPE,
	LEFT_CHEV,
	RIGHT_CHEV,
	DB_RIGHT_CHEV,
	DB_LEFT_CHEV,
	EXPAND,
};

# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define UNDERSCORE 95
# define SPECIAL_EXIT_CODE 255

/* ******************************* STRUCTURES ******************************* */

typedef struct s_parser {
	bool	case_single;
	bool	case_double;
	bool	case_quotes;
	int		vpos;
	char	*var;
}	t_parser;

typedef struct s_command {
	char			*word;
	int				type;
}	t_command;

typedef	struct s_export {
	int		envp_len;
	int		len;
	char	**new_envp;
	char	*equal;
	char	*var_name;
	char	*new_var;
	char	*formatted_value;
}	t_export;

typedef struct s_minishell {
	int					pipe_fd[2];
	int					heredoc_fd[2];
	int					com[2];
	int					fd;
	int					code_status;
	int					redir;
	pid_t				pid;
	char				*line;
	char				*h_line;
	char				*path;
	char				*last_cd_path;
	char				*cd_path;
	char				**cmd_args;
	char				**h_delimiter;
	char				**envp;
	struct s_command	*command;
	struct s_parser		*parser;
}	t_minishell;

extern int	g_sigstatus;

/* ******************************* LEXER ******************************* */

int			special_types(char c1, char c2);
int			is_in_quote(char c, t_parser *quotes);
int			error_quotes(t_minishell **main);
int			ft_error_lexer(t_command *command, t_minishell **main);

/* ******************************* PARSER ******************************* */

/* ------------------------------ INIT ------------------------------ */

t_minishell	*get_minishell(void);
t_minishell	*init_minishell(char **envp);
t_parser	*get_parser(t_alloc **garbage);
t_parser	*init_parser(t_alloc **garbage);
void		restore_minishell();
char		**set_env(char **envp, t_alloc **garbage);
int			is_only_quotes(char *line, int *i);
void		init_get_token(t_command *token);
void		init_get_expand(t_minishell **main, t_command *token, int *i, t_alloc **garbage);

/* ------------------------------ EXPAND ------------------------------ */

int			is_expand(char *line);
char		*get_env_var_name(char *line, int *i, t_alloc **garbage);
char		*ft_getenv(t_minishell **main, const char *name);
int			parse_expand_quotes(t_minishell **main, char *line, int *i);
int			get_lex_expand(t_minishell **main, int *i, t_command *token, t_alloc **garbage);

/* ------------------------------ MAIN ------------------------------ */

void		get_token_type(t_minishell **main, t_command *token);
t_command	get_token(t_minishell **main, int *i, t_alloc **garbage);
t_command	*get_command(t_minishell **main, t_alloc **garbage);
t_command	*ft_parsing(t_minishell **main, t_alloc **garbage);

/* ******************************* EXECUTION ******************************* */

/* ------------------------------ PATH ------------------------------*/

char		*find_command_in_segment(char *segment, char *command, t_alloc **garbage);
char		*find_command_path(char *command, t_alloc **garbage);

/* ------------------------------ COMMANDS ------------------------------*/

int			execute_builtins(t_minishell **main, t_alloc **garbage);
int			execute_non_builtin(t_minishell **main, t_alloc **garbage);
void		execute_command(t_minishell **main, t_alloc **garbage);

/* ------------------------------ MAIN ------------------------------ */

int			ft_heredoc(t_minishell **main, int *i, t_alloc **garbage);
// void		ft_heredoc_args(t_minishell **main, int i, t_alloc **garbage);
// int			heredoc_child(t_minishell **main, int *i, t_alloc **garbage);
char		*get_right_input(t_minishell **main, int *i, t_alloc **garbage);
int			ft_redirect(t_minishell **main, int *i, t_alloc **garbage);
int 		ft_pipex(t_minishell **main, int *i, t_alloc **garbage);
char		**create_cmd_args(t_minishell **main, int *i, t_alloc **garbage);
void		handle_command(t_minishell **main, t_alloc **garbage);


/* ******************************* BUILTINS ******************************* */

int			ft_cd(t_minishell **main);
int			ft_echo(t_minishell **main);
int			ft_env(t_minishell **main);
int			ft_exit(t_minishell **main, t_alloc **garbage);
int			ft_pwd(t_minishell **main);
int			ft_unset(t_minishell **main, char **names);

/* ------------------------------ FT_EXPORT ------------------------------ */

int			envp_length(char **envp);
char		**copy_envp(char **envp, int new_size, t_alloc **garbage);
void		add_or_update_env_var(char **envp, char *var, t_alloc **garbage);
bool		search_identifiers(const char *str, char *ptr, bool *equals, bool *no_space);
int 		ft_export(t_minishell **main, t_alloc **garbage);

/* ******************************* SIGNALS ******************************* */

void		child_handler(int signum);
int			process_prompt(void);
void		sigint_handler(int signum);
void		sigint_process_handler(int signum);
int			init_sigquit(void);
int			init_parent_signals(void);
void		sigquit_handler(int signum);
void		sig_process_handler(int signum);
void		init_signal(void);
void		init_process_signal(void);

/* ******************************* UTILS ******************************* */

t_command	token_null(t_command *token, t_alloc **garbage);
t_command	*ft_struct_join(t_command *tok1, t_command tok2, t_alloc **garbage);
char		*char_to_str(char c, t_alloc **garbage);
char		**ft_envjoin(char **envp, char *str, t_alloc **garbage);
int			ft_count(t_command *command, int *i);
int			is_builtin(char *command);
int			heredoc_is_expand(char *line);
int			replace_var(t_minishell **main, char **new_line, int *i, t_alloc **garbage);
char		*heredoc_get_expand(t_minishell **main, t_alloc **garbage);
char		**get_delimiter(t_minishell **main, int *i, t_alloc **garbage);
void		read_add(t_minishell **main, int *j, t_alloc **garbage);
void		check_spaces(t_minishell **main, int *i);
int			ft_strcmp_var(const char *s1, const char *s2);
int			is_input(t_minishell **main, int *i);
int			is_output(t_minishell **main, int *i);
int			redir_input(char *filename);
int			redir_append(char *filename);
int			redir_output(char *filename);

#endif