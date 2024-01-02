/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/02 13:26:12 by jdufour          ###   ########.fr       */
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
	bool	found;

	i = 0;
	len = 0;
	envp_len = 0;
	new_envp = NULL;
	found = false;
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
			break ;
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
	const char	*ptr;
	bool		equals_encountered;
	bool		non_space_after_equals;

	equals_encountered = false;
	non_space_after_equals = false;
	if (!str || !(*str == '_' || ft_isalpha(*str)))
		return (false);
	ptr = str + 1;
	while (*ptr)
	{
		if (*ptr == '=')
		{
			equals_encountered = true;
		}
		else if (equals_encountered)
		{
			if (!non_space_after_equals && *ptr != ' ')
				non_space_after_equals = true;
		}
		else if (non_space_after_equals && *ptr == ' ')
		{
			return (false);
		}
		else if (!(*ptr == '_' || ft_isalnum(*ptr)))
		{
			return (false);
		}
		ptr++;
	}
	return (true);
}


void	handle_value_case(char ***envp, char *var_name, char *value, t_alloc *garbage)
{
	char	*new_var;
	char	*formatted_value;

	if (!value || *value == '\0' || *value == ' ')
	{
		new_var = ft_strjoin(var_name, "=\"\"", garbage);
		add_or_update_env_var(envp, new_var, garbage);
	}
	else
	{
		formatted_value = ft_strjoin("\"", value, garbage);
		formatted_value = ft_strjoin(formatted_value, "\"", garbage);
		new_var = ft_strjoin(var_name, "=", garbage);
		new_var = ft_strjoin(new_var, formatted_value, garbage);
	}
	add_or_update_env_var(envp, new_var, garbage);
}


int ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
{
	int		i;
	char	*equal;
	char	*identifier;

	equal = NULL;
	identifier = NULL;
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
			printf("equal: %s\n", equal);
			if (equal)
			{
				*equal = '\0';
				identifier = argv[i];
				printf("identifier: %s\n", identifier);
				if (is_valid_identifier(identifier))
				{
					handle_value_case(envp, identifier, equal + 1, garbage);
					printf("identifier apres handle: %s\n", identifier);
				}
				else
					printf("export:`%s': not a valid identifier\n", identifier);
				*equal = '=';
			}
			else
			{
				if (is_valid_identifier(argv[i]))
					add_or_update_env_var(envp, argv[i], garbage);
				else
					printf("export: '%s': not a valid identifier\n", argv[i]);
			}
			i++;
		}
	}
	code->code_status = 0;
	return (code->code_status);
}

// int	ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
// {
// 	int		i;
// 	char	*equal;
// 	char	*identifier;

// 	equal = NULL;
// 	identifier = NULL;
// 	if (argv[1] == NULL)
// 	{

// 		i = 0;
// 		while ((*envp)[i] != NULL)
// 		{
// 			equal = ft_strchr((*envp)[i], '=');
// 			if (equal && *(equal + 1) == '\0')
// 				printf("export %.*s=\"\"\n", (int)(equal - (*envp)[i]), (*envp)[i]);
// 			else
// 				printf("export %s\n", (*envp)[i]);
// 			i++;
// 		}
// 		code->code_status = 0;
// 		return (code->code_status);
// 	}
// 	else
// 	{
// 		i = 1;
// 		while (argv[i] != NULL)
// 		{
// 			identifier = argv[i];
// 			if (!is_valid_identifier(identifier))
// 			{
// 				printf("export: '%s': not a valid identifier\n", argv[i]);
// 			}
// 			if (is_valid_identifier(identifier))
// 				add_or_update_env_var(envp, argv[i], garbage);
// 			i++;
// 		}
// 	}
// 	code->code_status = 0;
// 	return (code->code_status);
// }
