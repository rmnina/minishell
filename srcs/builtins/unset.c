/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:19:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 14:49:11 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	shift_env_variables(char ***envp, int deleted_index)
{
	int	j;

	j = deleted_index;
	while ((*envp)[j] != NULL)
	{
		(*envp)[j] = (*envp)[j + 1];
		j++;
	}
}

int	unset_single_variable(char ***envp, const char *name)
{
	int	i;
	int	len;
	int	found;

	len = ft_strlen(name);
	i = 0;
	found = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], name, len) == 0 && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			shift_env_variables(envp, i);
			found = 1;
			break ;
		}
		i++;
	}
	return (found);
}


int	ft_unset(char ***envp, char **names, t_code *code)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (names[i] != NULL)
	{
		if (unset_single_variable(envp, names[i]))
			found = 1;
		i++;
	}
	if (found >= 1)
		code->code_status = 0;
	code->code_status = 1;
	return (code->code_status);
}


