/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/02 21:30:04 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This function scans the token following the $ to retrieve the
// name of the expand, as it is required by the getenv function.

char	*get_env_var_name(char *line, int *i, t_alloc *garbage)
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

void	get_next_part_env_var(t_quotes *quotes, int j)
{
	if (quotes->var[j] == '\0')
	{
		quotes->var = NULL;
		quotes->vpos = 0;
	}
	else
		quotes->vpos = j;
}

// This function creates the token corresponding to the environnement 
// variable retrieved by getenv. If the variable is composed of more
// than one word, this function will be called again by get_command()
// thanks to the elements modified in the t_quotes structs by the
// previous function.

int	get_lex_expand(t_main_items *main, int *i, t_quotes *quotes, t_alloc *garbage)
{
	int		j;

	j = quotes->vpos;
	init_get_expand(&main->token, main->line, i, quotes, garbage, main->envp);
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
			main->token.word = ft_strjoin_char(main->token.word, quotes->var[j], garbage);
			j++;
		}
	}
	main->token.type = WORD;
	get_next_part_env_var(quotes, j);
	return (1);
}