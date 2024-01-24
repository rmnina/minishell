/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 05:00:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 20:01:08 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_identifier(char *str)
{
	char	*ptr;
	bool	equals;
	bool	no_space;

	ptr = NULL;
	equals = false;
	no_space = false;
	if (!str)
		return (false);
	if (*str == ' ' || *str == '=' || !(*str == '_' || ft_isalpha(*str)))
	{
		printf("export: `%s': not a valid identifier\n", str);
		return (false);
	}
	ptr = str + 1;
	while (*ptr)
	{
		if (!identifiers(str, ptr, &equals, &no_space))
			return (false);
		if (*ptr == '+' && *(ptr + 1) == '=')
			return (true);
		ptr++;
	}
	return (true);
}

void	compare_values(t_export *export, char **value, t_alloc **garbage)
{
	if (!**value || **value == '\0')
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
	else if (**value == '$')
	{
		export->formatted_value = ft_g_strjoin("\\", *value, PARSING, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
		export->new_var = \
		ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
	else if (**value == '"' || **value == '\'')
	{
		export->formatted_value = ft_g_strdup(*value, PARSING, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
		export->new_var = \
		ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
	else
	{
		export->formatted_value = ft_g_strdup(*value, PARSING, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
		export->new_var = \
		ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
}

void	export_append(t_minishell **main, char *var_name, \
char *value_to_append, t_alloc **garbage)
{
	char	*old_value;
	char	*new_value;
	char	*export_str;

	old_value = getenv(var_name);
	new_value = NULL;
	if (old_value && value_to_append)
		new_value = ft_g_strjoin(old_value, value_to_append, ENV, garbage);
	else
		new_value = ft_g_strdup(value_to_append, ENV, garbage);
	export_str = ft_g_strjoin(var_name, "=", ENV, garbage);
	export_str = ft_g_strjoin(export_str, new_value, ENV, garbage);
	add_or_update_env_var((*main)->envp, export_str, garbage);
}

void	export_variable(t_minishell **main, t_alloc **garbage)
{
	char	*equal;
	int		i;

	equal = NULL;
	i = 1;
	while ((*main)->cmd_args[i] != NULL)
	{
		equal = ft_strchr((*main)->cmd_args[i], '=');
		if (equal)
		{
			if (equal && is_valid_identifier((*main)->cmd_args[i]))
				handle_value_case(main, (*main)->cmd_args[i], garbage);
		}
		else
		{
			if (is_valid_identifier((*main)->cmd_args[i]))
				add_or_update_env_var \
				((*main)->envp, (*main)->cmd_args[i], garbage);
		}
		i++;
	}
}

int	ft_export(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[1] == NULL)
		return (ft_env(main));
	else
		export_variable(main, garbage);
	(*main)->code_status = 0;
	return ((*main)->code_status);
}
