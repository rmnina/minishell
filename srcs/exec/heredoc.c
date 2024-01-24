/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 19:45:22 by jdufour          ###   ########.fr       */
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
