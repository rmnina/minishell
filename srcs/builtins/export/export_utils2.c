/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:18:24 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 22:38:23 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	handle_plus_equal_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	char	*plus_equal;
	char	*var_name;
	char	*value_to_append;
	char	original_char;

	plus_equal = ft_strstr(arg, "+=");
	if (plus_equal)
	{
		original_char = *plus_equal;
		*plus_equal = '\0';
		var_name = ft_g_strndup(arg, plus_equal - arg, ENV, garbage);
		value_to_append = plus_equal + 2;
		if (is_valid_identifier(var_name))
		{
			export_append(main, var_name, value_to_append, garbage);
		}
		*plus_equal = original_char;
		return (0);
	}
	return (1);
}

void	handle_add_update_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	t_export	export;
	char		*value;

	export = init_export_struct();
	export.equal = ft_strchr(arg, '=');
	if (!export.equal)
	{
		if (is_valid_identifier(arg))
		{
			export.new_var = ft_g_strjoin(arg, "=", ENV, garbage);
			add_or_update_env_var((*main)->envp, export.new_var, garbage);
		}
		return ;
	}
	export.var_name = ft_g_strndup(arg, export.equal - arg, ENV, garbage);
	value = export.equal + 1;
	compare_values(&export, &value, garbage);
	add_or_update_env_var((*main)->envp, export.new_var, garbage);
}

void	handle_value_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	int	variable_modified;

	variable_modified = handle_plus_equal_case(main, arg, garbage);
	if (variable_modified == 0)
		return ;
	handle_add_update_case(main, arg, garbage);
}
