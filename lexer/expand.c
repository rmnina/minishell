/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/06 18:58:28 by jdufour          ###   ########.fr       */
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
	*i += 1;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == UNDERSCORE))
	{
		name = ft_strjoin_char(name, line[*i]);
		*i += 1;
	}
	return (name);
}

int	get_lex_expand(char *line, int *i, t_quotes *quotes, t_command *token)
{
	int		j;

	j = quotes->vpos;
	init_get_expand(token, line, i, quotes);
	if (quotes->var == NULL)
		return (-1);
	while (quotes->var[j])
	{
		is_in_quote(quotes->var[j], quotes);
		if (parse_quotes(quotes->var, &j, quotes) == 1)
		{
			j++;
			break ;
		}
		else if (!parse_quotes(quotes->var, &j, quotes))
		{
			token->word = ft_strjoin_char(token->word, quotes->var[j]);
			j++;
		}
	}
	token->type = WORD;
	if (quotes->var[j] == '\0')
	{
		quotes->var = NULL;
		quotes->vpos = 0;
	}
	else
		quotes->vpos = j;
	return (1);
}