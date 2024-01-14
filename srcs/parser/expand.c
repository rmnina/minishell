/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 16:52:52 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This function scans the token following the $ to retrieve the
// name of the expand, as it is required by the getenv function.

char	*get_env_var_name(char *line, int *i, t_alloc **garbage)
{
	char	*name;

	name = NULL;
	*i += 1;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == UNDERSCORE))
	{
		name = ft_strjoin_char(name, line[*i], garbage);
		*i += 1;
	}
	return (name);
}

// This function will be used in get_lex_expand(). It lets us know
// whether or not the name of the environnment variable retrieved 
// by getenv is a multi-part one.

void	get_next_part_env_var(t_parser *parser, int *j)
{
	if (!parser->var[*j])
	{
		parser->var = NULL;
		parser->vpos = 0;
	}
	else
		parser->vpos = *j;
}

// This function creates the token corresponding to the environnement 
// variable retrieved by getenv. If the variable is composed of more
// than one word, this function will be called again by get_command()
// thanks to the elements modified in the t_parser structs by the
// previous function.

int	get_lex_expand(t_minishell **main, int *i, t_command *token, t_alloc **garbage)
{
	int		j;

	j = (*main)->parser->vpos;
	init_get_expand(main, token, i, garbage);
	if ((*main)->parser->var == NULL)
		return (-1);
	while ((*main)->parser->var[j])
	{
		if ((*main)->parser->var[j] == 32)
		{
			j++;
			break ;
		}
		else if ((*main)->parser->var[j] != 32)
		{
			token->word = ft_strjoin_char(token->word, (*main)->parser->var[j], garbage);
			j++;
		}
	}
	token->type = WORD;
	get_next_part_env_var((*main)->parser, &j);
	return (1);
}
