/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 06:32:53 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/11 12:36:48 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_is_expand(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
				return (1);
			if (ft_isalnum(line[i + 1]) || line[i + 1] == UNDERSCORE)
				return (1);
		}
		i++;
	}
	return (0);
}

int	replace_var(t_minishell **main, char **new_line, int *i, t_alloc **garbage)
{
	char	*name;
	char	*var;

	name = NULL;
	var = NULL;
	if ((*main)->h_line[*i] == '$')
	{
		if ((*main)->h_line[*i + 1] && (*main)->h_line[*i + 1] == '?')
		{
			*i += 2;
			*new_line = ft_strjoin(*new_line, ft_itoa((*main)->code_status), garbage);
		}
		else if ((*main)->h_line[*i + 1] && (ft_isalnum((*main)->h_line[*i + 1]) || (*main)->h_line[*i + 1] == UNDERSCORE))
		{
			name = get_env_var_name((*main)->h_line, i, garbage);
			var = ft_getenv(main, name);
			if (var)
				*new_line = ft_strjoin(*new_line, var, garbage);
			(*i)--;
		}
		return (1);
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
			new_line = ft_strjoin_char(new_line, (*main)->h_line[i], garbage);
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
	(*main)->h_delimiter = garb_malloc(sizeof(char *), size + 1, garbage);
	while ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		(*i)++;
		(*main)->h_delimiter[j] = ft_strdup((*main)->command[*i].word, garbage);
		if (!(*main)->h_delimiter[j])
			return (NULL);
		j++;
		(*i)++;
	}
	return ((*main)->h_delimiter);
}

void	read_add(t_minishell **main, int *j, t_alloc **garbage)
{
	while (1)
	{
		(*main)->h_line = readline("> ");
		if (!(*main)->h_line || ft_strcmp((*main)->h_line, (*main)->h_delimiter[*j]) == 0)
			break ;
		if (heredoc_is_expand((*main)->h_line))
			(*main)->h_line = heredoc_get_expand(main, garbage);
		write((*main)->pipe_fd[1], (*main)->h_line, ft_strlen((*main)->h_line));
		write((*main)->pipe_fd[1], "\n", 1);
		free((*main)->h_line);
	}
}
