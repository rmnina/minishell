/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/11 10:42:10 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**set_env(char **envp, t_alloc **garbage)
{
	char	pwd[PATH_MAX];

	envp = ft_envjoin(envp, ft_strjoin("PWD=", getcwd(pwd, sizeof(pwd)), garbage), garbage);
	envp = ft_envjoin(envp, ft_strjoin("SHLVL=", "1", garbage), garbage);
	envp = ft_envjoin(envp, ft_strjoin("_=", "/usr/bin/env", garbage), garbage);
	return (envp);
}

char	*ft_getenv(t_minishell **main, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while ((*main)->envp[i] != NULL)
	{
		if (ft_strncmp((*main)->envp[i], name, len) == 0 && (*main)->envp[i][len] == '=')
			return &((*main)->envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

void	init_get_expand(t_minishell **main, t_command *token, int *i, t_alloc **garbage)
{
	char	*name;

	if ((*main)->parser->var == NULL)
	{
		name = get_env_var_name((*main)->line, i, garbage);
		(*main)->parser->var = ft_getenv(main, name);
	}
	if (!token->word)
		token->word = NULL;
	token->type = 0;
}

void	init_get_token(t_command *token)
{
	token->word = NULL;
	token->type = 0;
}