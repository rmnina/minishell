/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/28 19:27:35 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

int	handle_h_code_status(t_minishell **main, char **new_line, int *i, \
t_alloc **garbage)
{
	*i += 2;
	*new_line = ft_g_strjoin(*new_line, ft_itoa((*main)->code_status), \
	EXEC, garbage);
	return (1);
}

int	get_h_expand_var(t_minishell **main, char **new_line, int *i, \
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
			return (handle_h_code_status(main, new_line, i, garbage));
		else if ((*main)->h_line[*i + 1] && \
		(ft_isalnum((*main)->h_line[*i + 1]) \
		|| (*main)->h_line[*i + 1] == UNDERSCORE))
			return (get_h_expand_var(main, new_line, i, garbage));
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
		if (!replace_var(main, &new_line, &i, garbage) \
		&& (*main)->h_line[i] != '$')
			new_line = ft_strjoin_char(new_line, \
			(*main)->h_line[i], EXEC, garbage);
		i++;
	}
	return (new_line);
}
