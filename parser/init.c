/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/21 19:21:11 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_get_expand(t_command *token, char *line, int *i, t_quotes *quotes, t_alloc *garbage)
{
	char	*name;

	if (quotes->var == NULL)
	{
		name = get_env_var_name(line, i, garbage);
		quotes->var = getenv(name);
		// free(name);
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

t_command	*ft_parsing(char *line, t_alloc *garbage)
{
	t_quotes	quotes;
	t_command	*command;

	quotes.case_double = FALSE;
	quotes.case_single = FALSE;
	quotes.var = NULL;
	quotes.vpos = 0;
	if (error_quotes(line, &quotes) == -1)
		return (command = NULL);
	command = get_command(line, &quotes, garbage);
	if (ft_error_lexer(command) == -1)
		return (command = NULL);
	return (command);
}
