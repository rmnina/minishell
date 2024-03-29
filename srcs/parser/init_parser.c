/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 20:18:37 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:11:16 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**set_env(char **envp, t_alloc **garbage)
{
	char	pwd[PATH_MAX];

	envp = ft_envjoin(envp, ft_g_strjoin("PWD=", \
	getcwd(pwd, sizeof(pwd)), ENV, garbage), ENV, garbage);
	envp = ft_envjoin(envp, ft_g_strjoin("SHLVL=", \
	"1", ENV, garbage), ENV, garbage);
	envp = ft_envjoin(envp, ft_g_strjoin("_=", \
	"/usr/bin/env", ENV, garbage), ENV, garbage);
	envp = ft_envjoin(envp, ft_g_strjoin("PATH=", \
	"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin: \
	/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", \
	ENV, garbage), ENV, garbage);
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
		if (ft_strncmp((*main)->envp[i], name, len) == 0 \
		&& (*main)->envp[i][len] == '=')
			return (&((*main)->envp[i][len + 1]));
		i++;
	}
	return (NULL);
}

void	init_get_expand(t_minishell **main, t_command *token, \
int *i, t_alloc **garbage)
{
	char	*name;

	if ((*main)->parser->var == NULL)
	{
		name = get_env_var_name((*main)->line, i, garbage);
		is_in_quote((*main)->line[*i], (*main)->parser);
		(*main)->parser->var = ft_getenv(main, name);
	}
	if (!token->word)
		token->word = NULL;
	token->type = 0;
}

int	is_only_quotes(char *line, int *i)
{
	int	j;

	j = *i;
	if (line[j] == SINGLE_QUOTE)
	{
		while (line[j] == SINGLE_QUOTE)
			j++;
		if (line[j] == SPACE)
		{
			*i = j;
			return (1);
		}
	}
	j = *i;
	if (line[j] == DOUBLE_QUOTE)
	{
		while (line[j] == DOUBLE_QUOTE)
			j++;
		if (line[j] == SPACE)
		{
			*i = j;
			return (1);
		}
	}
	return (0);
}

void	init_get_token(t_command *token)
{
	token->word = NULL;
	token->type = 0;
}
