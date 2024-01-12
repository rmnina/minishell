/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/12 16:34:27 by jdufour          ###   ########.fr       */
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

int	expand_is_in_quote(char c, t_parser *parser)
{
	parser->case_double = TRUE;
	if (c == SINGLE_QUOTE && parser->case_single == FALSE \
	&& parser->case_double == FALSE)
		parser->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && parser->case_double == FALSE \
	&& parser->case_single == FALSE)
		parser->case_double = TRUE;
	else if (c == SINGLE_QUOTE && parser->case_single == TRUE)
		parser->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && parser->case_double == TRUE)
		parser->case_double = FALSE;
	if (parser->case_single == TRUE || parser->case_double == TRUE)
	{
		parser->case_quotes = TRUE;
		return (1);
	}
	else
	{
		parser->case_quotes = FALSE;
		return (0);
	}
}

int	parse_expand_quotes(t_minishell **main, char *line, int *i)
{
	if ((line[*i] == 32 && (*main)->parser->case_single == FALSE) || line[*i] == '\0')
		return (1);
	else if (line[*i] == DOUBLE_QUOTE && (*main)->parser->case_single == FALSE)
		return (*i += 1, 3);
	return (0);
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
		if (parse_expand_quotes(main, (*main)->parser->var, &j) == 1)
		{
			j++;
			break ;
		}
		else if (!parse_expand_quotes(main, (*main)->parser->var, &j))
		{
			token->word = ft_strjoin_char(token->word, (*main)->parser->var[j], garbage);
			j++;
		}
	}
	token->type = WORD;
	get_next_part_env_var((*main)->parser, &j);
	return (1);
}
