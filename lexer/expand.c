/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 17:40:42 by jdufour          ###   ########.fr       */
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

int	command_size(t_command *lexer, char *var, t_expand *expand)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (lexer[i].type && lexer[i].type != EXPAND)
		i++;
	expand->lex_index = i;
	if (lexer[i].type == EXPAND)
	{
		while (lexer[i].word[size])
			size++;
	}
	else
		return (0);
	return (size + (expand_size(var) - 1));
}

char	*get_var_name(t_command *lexer, t_expand *expand, char *name, t_quotes *quotes)
{
	int		i;
	int		j;
	int		size;

	i = expand->lex_index;
	j = 0;
	size = 0;
	while (lexer[i].word[j] && lexer[i].word[j] != EXPAND)
		j++;
	if (lexer[i].word[j] == EXPAND)
		j++;
	while (ft_isalnum(lexer[i].word[j] || lexer[i].word[j] == UNDERSCORE))
		size++;
	name = malloc(sizeof(char) * (size + 1));
	if (!name)
		return (NULL);
	size = 0;
	while (ft_isalnum(lexer[i].word[i] || lexer[i].word[i] == UNDERSCORE))
		name[size++] = lexer[i].word[j++];
	name[size] = '\0';
	return (name);
}