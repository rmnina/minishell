/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 19:28:04 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_var_name(char *line, int *i, t_alloc **garbage)
{
	char	*name;

	name = NULL;
	*i += 1;
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == UNDERSCORE))
	{
		name = ft_strjoin_char(name, line[*i], PARSING, garbage);
		*i += 1;
	}
	return (name);
}

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

int	get_lex_expand(t_minishell **main, int *i, \
t_command *token, t_alloc **garbage)
{
	int		j;

	j = (*main)->parser->vpos;
	init_get_expand(main, token, i, garbage);
	if ((*main)->parser->var == NULL)
	{
		if (token->word != NULL)
			return (0);
		else
			return ((*main)->parser->vpos = -1);
	}
	while ((*main)->parser->var[j])
	{
		if ((*main)->parser->var[j] == 32 || !(*main)->parser->var[j])
		{
			j++;
			break ;
		}
		else if ((*main)->parser->var[j] != 32)
		{
			token->word = ft_strjoin_char(token->word, \
			(*main)->parser->var[j], PARSING, garbage);
			j++;
		}
	}
	token->type = WORD;
	get_next_part_env_var((*main)->parser, &j);
	return (1);
}
