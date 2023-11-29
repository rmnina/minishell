/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 13:25:44 by jdufour          ###   ########.fr       */
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

	i = 0;
	while (lexer[i].word)
	{
		i++;
	}
	return (i + (expand_size(var) - 1));
}

char	*get_var_name(t_command *lexer, int *i, char *name)
{
	int		j;
	int		size;

	j = 0;
	size = 0;
	while (lexer[*i].word && lexer[*i].word[j] != EXPAND)
		j++;
	if (lexer[*i].word[j] != EXPAND)
	{
		j++;
		size = j;
		while (lexer[*i].word[size])
			size++;
	}
	name = malloc(sizeof(char) * (size - j) + 1);
	if (!name)
		return (NULL);
	size = 0;
	while (lexer[*i].word[j])
		name[size++] = lexer[*i].word[j++];
	name[size] = '\0';
	return (name);
}