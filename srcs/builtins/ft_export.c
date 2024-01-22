/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 05:00:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/22 16:37:55 by juandrie         ###   ########.fr       */
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
		if (!search_identifiers(str, ptr, &equals, &no_space))
			return (false);
		ptr++;
	}
	return (true);
}

void	compare_values(t_export *export, char **value, t_alloc **garbage)
{
	if (!**value || **value == '\0')
		export->new_var = ft_strjoin(export->var_name, "=", garbage);
	else if (**value == '$')
	{
		export->formatted_value = ft_strjoin("\\", *value, garbage);
		export->new_var = ft_strjoin(export->var_name, "=", garbage);
		export->new_var = \
		ft_strjoin(export->new_var, export->formatted_value, garbage);
	}
	else if (**value == '"' || **value == '\'')
	{
		export->formatted_value = ft_strdup(*value, garbage);
		export->new_var = ft_strjoin(export->var_name, "=", garbage);
		export->new_var = \
		ft_strjoin(export->new_var, export->formatted_value, garbage);
	}
	else
	{
		export->formatted_value = ft_strdup(*value, garbage);
		export->new_var = ft_strjoin(export->var_name, "=", garbage);
		export->new_var = \
		ft_strjoin(export->new_var, export->formatted_value, garbage);
	}
}
// void	export_append(t_minishell **main, char *var_name, char *value_to_append, t_alloc **garbage)
// {
// 	char	*old_value;
// 	char	*new_value;
// 	char	*export_str;
// 	char	*final_export;

// 	old_value = getenv(var_name);
// 	new_value = NULL;
// 	if (old_value)
// 		new_value = ft_strjoin(old_value, value_to_append, garbage);
// 	else
// 		new_value = ft_strdup(value_to_append, garbage);

// 	export_str = ft_strjoin(var_name, "=", garbage);
// 	final_export = ft_strjoin(export_str, new_value, garbage);
// 	add_or_update_env_var((*main)->envp, final_export, garbage);
// }



void	handle_value_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	t_export	export;
	char		*value;
	// char		*plus_equal;
	// char		*var_name;
	// char		*value_to_append;

	value = NULL;
	// plus_equal = ft_strchr(arg, "+=");
	// if (plus_equal)
	// {
	// 	*plus_equal = '\0';
	// 	var_name = ft_strndup(arg, plus_equal - arg, garbage);
	// 	value_to_append = plus_equal + 2;
	// 	export_append(main, var_name, value_to_append, garbage);
	// }
	export.equal = ft_strchr(arg, '=');
	if (!export.equal)
	{
		if (is_valid_identifier(arg))
		{
			export.new_var = ft_strjoin(arg, "=", garbage);
			add_or_update_env_var((*main)->envp, export.new_var, garbage);
		}
		return ;
	}
	export.var_name = ft_strndup(arg, export.equal - arg, garbage);
	value = export.equal + 1;
	compare_values(&export, &value, garbage);
	add_or_update_env_var((*main)->envp, export.new_var, garbage);
}

void    export_variable(t_minishell **main, t_alloc **garbage)
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
