/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/28 18:07:11 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	ft_export(char **envp, t_code *code)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		printf("declare -x %s\n", envp[i]);
// 		i++;
// 	}
// 	code->code_status = 0;
// 	return (code->code_status);
// }

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


char	**copy_envp(char **envp, int new_size, t_alloc *garbage)
{
	int		i;
	char	**new_envp;

	i = 0;
	new_envp = malloc(sizeof(char *) * (new_size + 1));
	if (new_envp == NULL)
	{
		return (NULL);
	}
	while (i < new_size)
	{
		if (envp[i] != NULL)
		{
			new_envp[i] = ft_strdup(envp[i], garbage);
			if (new_envp[i] == NULL)
			{
				perror("Failed to duplicate string");
				while (i > 0)
				{
					free(new_envp[--i]);
				}
				free(new_envp);
				return (NULL);
			}
		}
		else
		{
			new_envp[i] = NULL;
		}
		i++;
	}
	new_envp[new_size] = NULL;
	return (new_envp);
}

void	add_or_update_env_var(char ***envp, char *var, t_alloc *garbage)
{
	int		len;
	int		envp_len;
	char	**new_envp;
	int		i;
	bool found = false;

	i = 0;
	len = 0;
	envp_len = 0;
	new_envp = NULL;
	while (var[len] != '=' && var[len] != '\0')
		len++;
	envp_len = envp_length(*envp);
	new_envp = copy_envp(*envp, envp_len + 1, garbage);
	if (new_envp == NULL)
		return ;
	while (i < envp_len)
	{
		if (ft_strncmp(new_envp[i], var, len) == 0 && new_envp[i][len] == '=')
		{
			free(new_envp[i]);
			new_envp[i] = ft_strdup(var, garbage);
			if (new_envp[i] == NULL)
			{
				free(new_envp);
				return ;
			}
			found = true;
            break;
			//free(*envp);
			//*envp = new_envp;
			//return ;
		}
		i++;
	}
	if (!found)
	{
		new_envp[envp_len] = ft_strdup(var, garbage);
		new_envp[envp_len + 1] = NULL;
	}
	*envp = new_envp;
}

bool	is_valid_identifier(const char *str)
{
	if (!str || !(*str == '_' || ft_isalpha(*str)))
		return (false);
	str++;
	while (*str)
	{
		if (!(*str == '_' || ft_isalnum(*str)))
			return (false);
		str++;
	}
	return (true);
}

int	ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
{
	int		i;
	char	*equal;
	char	*identifier;

	equal = NULL;
	if (argv[1] == NULL) 
	{

		i = 0;
		while ((*envp)[i] != NULL)
		{
			equal = ft_strchr((*envp)[i], '=');
			if (equal && *(equal + 1) == '\0')
				printf("export %.*s=\"\"\n", (int)(equal - (*envp)[i]), (*envp)[i]);
			else
				printf("export %s\n", (*envp)[i]);
			i++;
		}
		code->code_status = 0;
		return (code->code_status);
	}
	else
	{
		i = 1;
		while (argv[i] != NULL)
		{
			equal = ft_strchr(argv[i], '=');
			if (equal != NULL)
			{
				identifier = ft_strndup(argv[i], equal - argv[i], garbage);
				printf("identifier: %s\n", identifier);
			}
			else
			{
				identifier = ft_strdup(argv[i], garbage);
				printf("identifier: %s\n", identifier);
			}
			if (!is_valid_identifier(identifier))
			{
				printf("export: '%s': not a valid identifier\n", identifier);
				code->code_status = 1;
                break ;
			}
			add_or_update_env_var(envp, argv[i], garbage);
			i++;
		}
	}
	code->code_status = 0;
	return (code->code_status);
}

