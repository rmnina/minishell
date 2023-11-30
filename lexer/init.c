/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/30 15:07:06 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*init_get_expand(t_command *token, char *line, int *i, t_expand *expand)
{
	char	*name;
	char	*var;

	name = get_env_var_name(line, i);
	var = getenv(name);
	expand->left_expand = FALSE;
	token->word = NULL;
	return (var);
}

void	init_get_token(t_command *token, t_expand *expand)
{
	expand->left_expand = FALSE;
	token->word = NULL;
}
