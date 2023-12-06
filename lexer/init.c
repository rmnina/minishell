/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/06 18:49:46 by jdufour          ###   ########.fr       */
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

void	init_get_token(t_command *token, t_expand *expand)
{
	expand->left_expand = FALSE;
	token->word = NULL;
	token->type = 0;
}
