/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/10 18:10:50 by jdufour          ###   ########.fr       */
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

void	get_next_part_env_var(t_quotes *quotes, int *j)
{
	if (!quotes->var[*j])
	{
		quotes->var = NULL;
		quotes->vpos = 0;
	}
	else
		quotes->vpos = *j;
}

int	expand_is_in_quote(char c, t_quotes *quotes)
{
	quotes->case_double = TRUE;
	if (c == SINGLE_QUOTE && quotes->case_single == FALSE \
	&& quotes->case_double == FALSE)
		quotes->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == FALSE \
	&& quotes->case_single == FALSE)
		quotes->case_double = TRUE;
	else if (c == SINGLE_QUOTE && quotes->case_single == TRUE)
		quotes->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == TRUE)
		quotes->case_double = FALSE;
	if (quotes->case_single == TRUE || quotes->case_double == TRUE)
	{
		quotes->case_quotes = TRUE;
		return (1);
	}
	else
	{
		quotes->case_quotes = FALSE;
		return (0);
	}
}


// This function creates the token corresponding to the environnement 
// variable retrieved by getenv. If the variable is composed of more
// than one word, this function will be called again by get_command()
// thanks to the elements modified in the t_quotes structs by the
// previous function.

int	get_lex_expand(char *line, int *i, t_quotes *quotes, t_command *token, t_alloc **garbage, char ***envp)
{
	int		j;

	j = quotes->vpos;
	init_get_expand(token, line, i, quotes, garbage, envp);
	if (quotes->var == NULL)
		return (-1);
	while (quotes->var[j])
	{
		if (parse_expand_quotes(quotes->var, &j, quotes) == 1)
		{
			j++;
			break ;
		}
		else if (!parse_expand_quotes(quotes->var, &j, quotes))
		{
			token->word = ft_strjoin_char(token->word, quotes->var[j], garbage);
			j++;
		}
	}
	token->type = WORD;
	get_next_part_env_var(quotes, &j);
	return (1);
}
