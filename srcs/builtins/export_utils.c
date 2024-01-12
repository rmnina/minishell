/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/12 16:47:28 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	envp_length(char **envp)
{
	int	length;

	length = 0;
	while (envp[length] != NULL)
	{
		length++;
	}
	return (length);
}

char	**copy_envp(char **envp, int new_size, t_alloc **garbage)
{
	int		i;
	char	**new_envp;

	i = 0;
	new_envp = garb_malloc(sizeof(char *), (new_size + 1), garbage);
	if (new_envp == NULL)
		return (NULL);
	while (i < new_size)
	{
		if (envp[i] != NULL)
		{
			new_envp[i] = ft_strdup(envp[i], garbage);
			if (new_envp[i] == NULL)
			{
				while (i > 0)
					free(new_envp[--i]);
				free(new_envp);
				return (NULL);
			}
		}
		else
			new_envp[i] = NULL;
		i++;
	}
	new_envp[new_size] = NULL;
	return (new_envp);
}

int	search_var(t_export *export, char *var, t_alloc **garbage)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (i < export->envp_len)
	{
		if (ft_strncmp(export->new_envp[i], var, export->len) == 0)
		{
			if (var[export->len] == '=')
			{
				free(export->new_envp[i]);
				export->new_envp[i] = ft_strdup(var, garbage);
				if (export->new_envp[i] == NULL)
				{
					free(export->new_envp);
					return (found = -1);
				}
			}
			found = 1;
			break ;
		}
		i++;
	}
	return (found);
}

void	add_or_update_env_var(char ***envp, char *var, t_alloc **garbage)
{
	t_export	export;
	int			found;

	export.len = 0;
	export.envp_len = 0;
	export.new_envp = NULL;
	while (var[export.len] != '=' && var[export.len] != '\0')
		export.len += 1;
	export.envp_len = envp_length(*envp);
	export.new_envp = copy_envp(*envp, export.envp_len + 1, garbage);
	if (export.new_envp == NULL)
		return ;
	found = search_var(&export, var, garbage);
	if (found == -1)
		return ;
	if (found == 0)
	{
		export.new_envp[export.envp_len] = ft_strdup(var, garbage);
		export.new_envp[export.envp_len + 1] = NULL;
	}
	*envp = export.new_envp;
}

bool	search_identifiers(const char *str, char *ptr, bool *equals, bool *no_space)
{
	if (*ptr == '=')
		*equals = true;
	else if (*equals)
	{
		if (!*no_space && *ptr != ' ')
			*no_space = true;
	}
	else if (*no_space && *ptr == ' ')
		return (false);
	else if (!(*ptr == '_' || ft_isalnum(*ptr)))
	{
		printf("export: `%s': not a valid identifier\n", str);
		return (false);
	}
	return (true);
}
