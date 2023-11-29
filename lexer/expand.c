/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 20:53:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_size(char *var)
{
	int			i;
	int			size;

	i = 0;
	size = 0;
	while (var[i])
	{
		if (var[i] == SPACE || (i == 0 && var[i] != SPACE))
		{
			if (var[i + 1] && var[i + 1] != SPACE)
				size++;
		}
		i++;
	}
	return (size);
}

int	command_size(t_command *lexer, char *var)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (lexer[i].type && lexer[i].type != EXPAND)
		i++;
	if (lexer[i].type == EXPAND)
	{
		while (lexer[i].word[size])
			size++;
	}
	else
		return (0);
	return (size + (expand_size(var) - 1));
}

char	*get_env_var_name(char *line, int *i)
{
	char	*name;

	name = NULL;
	while (ft_isalnum(line[*i]) || line[*i] != UNDERSCORE)
	{
		name = ft_strjoin_char(name, line[*i]);
		i++;
	}
	return (name);
}