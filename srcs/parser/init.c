/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/02 13:29:27 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getenv(char ***envp, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], name, len) == 0 && (*envp)[i][len] == '=')
		{
			return &((*envp)[i][len + 1]);
		}
		i++;
	}
	return (NULL);
}


void	init_get_expand(t_command *token, char *line, int *i, t_quotes *quotes, t_alloc *garbage, char ***envp)
{
	char	*name;

	if (quotes->var == NULL)
	{
		name = get_env_var_name(line, i, garbage);
		quotes->var = ft_getenv(envp, name);
		//free(name);
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

t_command	*ft_parsing(char *line, t_alloc *garbage, char ***envp)
{
	t_quotes	quotes;
	t_command	*command;

	quotes.case_double = FALSE;
	quotes.case_single = FALSE;
	quotes.var = NULL;
	quotes.vpos = 0;
	if (error_quotes(line, &quotes) == -1)
		return (command = NULL);
	command = get_command(line, &quotes, garbage, envp);
	if (ft_error_lexer(command) == -1)
		return (command = NULL);
	return (command);
}
