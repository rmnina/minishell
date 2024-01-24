/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/01/24 19:45:22 by jdufour          ###   ########.fr       */
=======
/*   Updated: 2024/01/24 18:12:13 by jdufour          ###   ########.fr       */
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_special_variable(t_minishell **main, char **new_line, int *i, \
t_alloc **garbage)
{
	*i += 2;
	*new_line = ft_g_strjoin(*new_line, ft_itoa((*main)->code_status), \
	EXEC, garbage);
	return (1);
}

int	handle_regular_variable(t_minishell **main, char **new_line, int *i, \
t_alloc **garbage)
{
	char	*name;
	char	*var;

	name = get_env_var_name((*main)->h_line, i, garbage);
	var = ft_getenv(main, name);
	if (var)
		*new_line = ft_g_strjoin(*new_line, var, EXEC, garbage);
	(*i)--;
	return (1);
}

int	replace_var(t_minishell **main, char **new_line, int *i, t_alloc **garbage)
{
	if ((*main)->h_line[*i] == '$')
	{
		if ((*main)->h_line[*i + 1] && (*main)->h_line[*i + 1] == '?')
		{
			*i += 2;
			*new_line = ft_g_strjoin(*new_line, ft_itoa((*main)->code_status), PARSING, garbage);
		}
		else if ((*main)->h_line[*i + 1] && \
		(ft_isalnum((*main)->h_line[*i + 1]) \
		|| (*main)->h_line[*i + 1] == UNDERSCORE))
		{
			name = get_env_var_name((*main)->h_line, i, garbage);
			var = ft_getenv(main, name);
			if (var)
				*new_line = ft_g_strjoin(*new_line, var, PARSING, garbage);
			(*i)--;
		}
	}
	return (0);
}

char	*heredoc_get_expand(t_minishell **main, t_alloc **garbage)
{
	int		i;
	char	*new_line;

	i = 0;
	new_line = NULL;
	while ((*main)->h_line[i])
	{
		if (!replace_var(main, &new_line, &i, garbage) && (*main)->h_line[i] != '$')
			new_line = ft_strjoin_char(new_line, (*main)->h_line[i], PARSING, garbage);
		i++;
	}
	return (new_line);
}

char	**get_delimiter(t_minishell **main, int *i, t_alloc **garbage)
{
	int		j;
	int		size;

	j = 0;
	size = 0;
	while ((*main)->command[*i + j].type == DB_LEFT_CHEV)
	{
		j += 2;
		size++;
	}
    j = 0;
	(*main)->h_delimiter = garb_malloc(sizeof(char *), size + 1, PARSING, garbage);
	while ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		(*i)++;
		(*main)->h_delimiter[j] = ft_g_strdup((*main)->command[*i].word, PARSING, garbage);
		if (!(*main)->h_delimiter[j])
			return (NULL);
		j++;
		(*i)++;
	}
	return ((*main)->h_delimiter);
}
<<<<<<< HEAD
=======

void	generate_temp_filename(t_minishell **main)
{
	int		fd;
	char	random_string[11];
	char	buffer;
	int		i;

	fd = open("/dev/urandom", O_RDONLY);
	i = 0;
	buffer = 0;
	while (i < 10)
	{
		read(fd, &buffer, 1);
		random_string[i] = (buffer % 26) + 'a';
		++i;
	}
	random_string[10] = '\0';
	close(fd);
	(*main)->tmp_filename = ft_strjoin("/tmp/heredoc_", random_string);
	if (!(*main)->tmp_filename)
		exit(EXIT_FAILURE);
}

void	init_temp_file(t_minishell **main)
{
	generate_temp_filename(main);
	(*main)->tmp_fd = open((*main)->tmp_filename, \
	O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if ((*main)->tmp_fd < 0)
		exit(EXIT_FAILURE);
}

void	read_add(t_minishell **main, int *j, t_alloc **garbage)
{
	while (42)
	{
		(*main)->h_line = readline("> ");
		if (!(*main)->h_line || \
		ft_strcmp((*main)->h_line, (*main)->h_delimiter[*j]) == 0)
			break ;
		if (heredoc_is_expand((*main)->h_line))
			(*main)->h_line = heredoc_get_expand(main, garbage);
		write((*main)->tmp_fd, (*main)->h_line, ft_strlen((*main)->h_line));
		write((*main)->tmp_fd, "\n", 1);
		free((*main)->h_line);
	}
}

int	ft_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	int	j;

	j = 0;
	(*main)->h_delimiter = get_delimiter(main, i, garbage);
	while ((*main)->h_delimiter[j])
	{	
		(*main)->tmp_fd = \
		open((*main)->tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if ((*main)->tmp_fd < 0)
			exit(EXIT_FAILURE);
		read_add(main, &j, garbage);
		j++;
		close((*main)->tmp_fd);
	}
	open((*main)->tmp_filename, O_RDONLY);
	if ((*main)->tmp_fd < 0)
		exit(EXIT_FAILURE);
	dup2((*main)->tmp_fd, STDIN_FILENO);
	close((*main)->tmp_fd);	
	return (1);
}

>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
