/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:03:22 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 18:05:19 by jdufour          ###   ########.fr       */
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
	PARSING,
	ENV,
};

# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define UNDERSCORE 95
# define SPECIAL_EXIT_CODE 255
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_RESET   "\x1b[0m"

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

typedef struct s_export {
	int		envp_len;
	int		len;
	char	**new_envp;
	char	*equal;
	char	*var_name;
	char	*new_var;
	char	*formatted_value;
}	t_export;

typedef struct s_alloc {
	void			*adr;
	int				cat;
	struct s_alloc	*next;
}	t_alloc;

typedef struct s_minishell {
	int					fd[2];
	int					infilefd;
	int					outfilefd;
	int					old_fd;
	int					code_status;
	int					status;
	int					tmp_fd;
	int					heredoc;
	int					heredoc_used;
	int					total_cmd;
	int					nb_cmd;
	pid_t				*pid;
	char				*tmp_filename;
	char				*line;
	char				*h_line;
	char				*path;
	char				*last_cd_path;
	char				*cd_path;
	char				**cmd_args;
	char				**h_delimiter;
	char				**envp;
	bool				is_heredoc_used;
	struct s_command	*command;
	struct s_parser		*parser;
}	t_minishell;

extern int	g_sigstatus;

/* ******************************* LEXER ******************************* */

int			special_types(t_minishell **main, int *i);
int			is_in_quote(char c, t_parser *quotes);
int			error_quotes(t_minishell **main);
int			ft_error_lexer(t_command *command, t_minishell **main);

/* ******************************* PARSER ******************************* */

/* ------------------------------ INIT ------------------------------ */

t_minishell	*get_minishell(void);
t_minishell	*init_minishell(char **envp);
t_parser	*get_parser(t_alloc **garbage);
t_parser	*init_parser(t_alloc **garbage);
void		restore_minishell(void);
char		**set_env(char **envp, t_alloc **garbage);
int			is_only_quotes(char *line, int *i);
void		init_get_token(t_command *token);
void		init_get_expand(t_minishell **main, t_command *token, int *i, t_alloc **garbage);

/* ------------------------------ EXPAND ------------------------------ */

int			is_expand(char *line);
char		*get_env_var_name(char *line, int *i, t_alloc **garbage);
char		*ft_getenv(t_minishell **main, const char *name);
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

int			init_pids(t_minishell **main, t_alloc **garbage);
int			ft_heredoc(t_minishell **main, int *i, t_alloc **garbage);
int			ft_redirect(t_minishell **main, int *i, t_alloc **garbage);
int			ft_pipex(t_minishell **main, int *i, t_alloc **garbage);
char		**create_cmd_args(t_minishell **main, int *i, t_alloc **garbage);
void		init_redirect(t_minishell **main, int *i, t_alloc **garbage);
void		handle_command(t_minishell **main, t_alloc **garbage);
int			get_all_inputs(t_minishell **main, int *i, t_alloc **garbage);
int			get_all_outputs(t_minishell **main, int *i, t_alloc **garbage);


/* ******************************* BUILTINS ******************************* */

int			ft_cd(t_minishell **main, t_alloc **garbage);
int			ft_echo(t_minishell **main);
int			ft_env(t_minishell **main);
int			ft_exit(t_minishell **main, t_alloc **garbage);
int			ft_pwd(t_minishell **main);
int			ft_unset(t_minishell **main, char **names, t_alloc **garbage);

/* ------------------------------ FT_EXPORT ------------------------------ */

int			envp_length(char **envp);
char		**copy_envp(char **envp, int new_size, t_alloc **garbage);
void		add_or_update_env_var(char **envp, char *var, t_alloc **garbage);
bool		identifiers(const char *str, char *ptr, bool *equals, bool *no_space);
int			ft_export(t_minishell **main, t_alloc **garbage);
void		handle_value_case(t_minishell **main, char *arg, t_alloc **garbage);
bool		is_valid_identifier(char *str);
void		export_append(t_minishell **main, char *var_name, char *value_to_append, t_alloc **garbage);
void		compare_values(t_export *export, char **value, t_alloc **garbage);

/* ******************************* SIGNALS ******************************* */

// void		child_handler(int signum);
// int			process_prompt(void);
void		sigint_handler(int signum);
void		sigint_process_handler(int signum);
// int			init_sigquit(void);
// int			init_parent_signals(void);
// void		sigquit_handler(int signum);
// void		sig_process_handler(int signum);
void		init_signal(void);
void		init_process_signal(void);
// int			init_sigactionsa(struct sigaction *sa);

/* ******************************* UTILS ******************************* */

/* ------------------------------ MEMORY ------------------------------ */

t_command	*ft_struct_join(t_command *tok1, t_command tok2, int cat, t_alloc **garbage);
char		**ft_envjoin(char **envp, char *str, int cat, t_alloc **garbage);
char		**ft_strjoin_args(t_minishell **main, int *i, int cat, t_alloc **garbage);
char		*ft_strjoin_char(char *s1, const char c, int cat, t_alloc **garbage);
char		*ft_g_strjoin(char *s1, const char *s2, int cat, t_alloc **garbage);
void		part_free_garb(t_alloc **garbage, int cat);
void		free_garbage(t_alloc **garbage);
t_alloc		*create_garbage_node(void *ptr, int cat);
t_alloc		*ft_garblast(t_alloc *last);
void		add_garbage_node(t_alloc **garbage, t_alloc *new);
void		*garb_malloc(size_t type, size_t size, int cat, t_alloc **garbage);
char		*ft_g_strndup(char *src, size_t n, int cat, t_alloc **garbage);
char		*ft_g_strdup(char *src, int cat, t_alloc **garbage);
void    	free_small_garb(t_alloc **garbage);
void    	free_adr(t_alloc **garbage, void *adr);

t_command	token_null(t_command *token, t_alloc **garbage);
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
int			redir_input(t_minishell **main, char *filename);
int			redir_append(t_minishell **main, char *filename);
int			redir_output(t_minishell **main, char *filename);
int			will_be_piped(t_minishell **main, int *i);
int			check_redir(t_minishell **main, int *i);
int 		check_next_redir(t_minishell **main, int *i);
void		check_next_args(t_minishell **main, int *i, t_alloc **garbage);
char		*get_last_out_filename(t_minishell **main, int *i, t_alloc **garbage);
char		*get_last_in_filename(t_minishell **main, int *i, t_alloc **garbage);
int			browse_outputs(t_minishell **main, int *i, char **filename, t_alloc **garbage);
int			browse_inputs(t_minishell **main, int *i, char **filename, t_alloc **garbage);
char		*ft_realpath(char *path, char *resolved_path, t_alloc **garbage);
char		*change_directory(t_minishell **main, char *path, t_alloc **garbage);
int			ft_cd_main(t_minishell **main, t_alloc **garbage);
#endif