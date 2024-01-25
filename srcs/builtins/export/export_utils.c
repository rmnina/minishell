/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 23:52:04 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	envp_length(char **envp)
{
	int	length;

	length = 0;
	while (envp[length] != NULL)
		length++;
	return (length);
}

int	search_var(char **envp, t_export *export, char *var, t_alloc **garbage)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (i < export->envp_len)
	{
		if (ft_strcmp_var(envp[i], var) == 0)
		{
			if (var[export->len] == '=')
			{
				envp[i] = ft_g_strdup(var, ENV, garbage);
				if (envp[i] == NULL)
					return (found = -1);
			}
			found = 1;
			break ;
		}
		i++;
	}
	return (found);
}

void	add_or_update_env_var(char **envp, char *var, t_alloc **garbage)
{
	t_export	export;
	int			found;

	export = init_export_struct();
	export.len = 0;
	export.envp_len = 0;
	while (var[export.len] != '=' && var[export.len] != '\0')
		export.len += 1;
	export.envp_len = envp_length(envp);
	found = search_var(envp, &export, var, garbage);
	if (found == -1)
		return ;
	if (found == 0)
	{
		envp[export.envp_len] = ft_g_strdup(var, ENV, garbage);
		envp[export.envp_len + 1] = NULL;
	}
}

bool	identifiers(const char *str, char *ptr, bool *equals, bool *no_space)
{
	int	i;

	i = 0;
	if (ptr[i] == '=')
	{
		*equals = true;
	}
	else if (*equals)
	{
		if (!*no_space && ptr[i] != ' ')
			*no_space = true;
	}
	else if (*no_space && ptr[i] == ' ')
		return (false);
	else if (!(ptr[i] == '_' || ft_isalnum(ptr[i]) || \
	(ptr[i] == '+' && ptr[i + 1] == '=')))
	{
		printf("export: `%s': not a valid identifier\n", str);
		return (false);
	}
	return (true);
}