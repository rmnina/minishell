/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 05:00:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 17:55:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_valid_identifier(char *str)
{
	char	*ptr;
	bool	equals;
	bool	no_space;

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
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
	else if (**value == '$')
	{
		export->formatted_value = ft_g_strjoin("\\", *value, PARSING, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
	    export->new_var = ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
	else if (**value == '"' || **value == '\'')
	{
		export->formatted_value = ft_g_strdup(*value, PARSING, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
	    export->new_var = ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
	else
	{
		export->formatted_value = ft_g_strdup(*value, PARSING, garbage);
		// export->formatted_value = ft_strdup(export->formatted_value, garbage);
		export->new_var = ft_g_strjoin(export->var_name, "=", PARSING, garbage);
	    export->new_var = ft_g_strjoin(export->new_var, export->formatted_value, PARSING, garbage);
	}
}

void	handle_value_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	t_export	export;
	char		*value;

	export.equal = ft_strchr(arg, '=');
	if (!export.equal)
	{
		if (is_valid_identifier(arg))
		{
			export.new_var = ft_g_strjoin(arg, "=", PARSING, garbage);
			add_or_update_env_var((*main)->envp, export.new_var, garbage);
		}
		return ;
	}
	export.var_name = ft_g_strndup(arg, export.equal - arg, PARSING, garbage);
	value = export.equal + 1;
	compare_values(&export, &value, garbage);
	add_or_update_env_var((*main)->envp, export.new_var, garbage);
}

void    export_variable(t_minishell **main, t_alloc **garbage)
{
    char	*equal;
	int 	i;

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
				add_or_update_env_var((*main)->envp, (*main)->cmd_args[i], garbage);
		}
		i++;
	}
}

int ft_export(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[1] == NULL)
		return (ft_env(main));
	else
        export_variable(main, garbage);
	(*main)->code_status = 0;
	return ((*main)->code_status);
}