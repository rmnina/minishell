/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/07 18:39:00 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_get_expand(t_command *token, char *line, int *i, t_quotes *quotes)
{
	char	*name;

	if (quotes->var == NULL)
	{
		name = get_env_var_name(line, i);
		quotes->var = getenv(name);
		free(name);
	}
	name = NULL;
	token->word = NULL;
	token->type = 0;
}

void	init_get_token(t_command *token)
{
	token->word = NULL;
	token->type = 0;
}

t_command	*ft_parsing(char *line)
{
	t_quotes	quotes;
	t_command	*command;
	//char		*line;

	quotes.case_double = FALSE;
	quotes.case_single = FALSE;
	quotes.var = NULL;
	quotes.vpos = 0;
	command = get_command(line, &quotes);
	ft_error_lexer(command);
	return (command);
}
