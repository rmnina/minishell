/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:18:24 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/23 19:26:30 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_plus_equal_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	t_export	export;
	char		*plus_equal;
	char		*var_name;
	char		*value_to_append;
	char		*value;

	plus_equal = ft_strstr(arg, "+=");
	if (plus_equal)
	{
		*plus_equal = '\0';
		var_name = ft_strndup(arg, plus_equal - arg, garbage);
		value_to_append = plus_equal + 2;
		if (is_valid_identifier(var_name))
			export_append(main, var_name, value_to_append, garbage);
		return ;
	}
	export.var_name = ft_strndup(arg, export.equal - arg, garbage);
	value = export.equal + 1;
	compare_values(&export, &value, garbage);
	add_or_update_env_var((*main)->envp, export.new_var, garbage);
}

void	handle_add_update_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	t_export	export;
	char		*value;

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

void	handle_value_case(t_minishell **main, char *arg, t_alloc **garbage)
{
	handle_plus_equal_case(main, arg, garbage);
	handle_add_update_case(main, arg, garbage);
}
