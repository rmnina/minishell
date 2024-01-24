/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:19:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 11:34:21 by juandrie         ###   ########.fr       */
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
	int	found;

	i = 0;
	found = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strcmp_var((*envp)[i], name) == 0)
		{
			shift_env_variables(envp, i);
			found = 1;
			break ;
		}
		i++;
	}
	return (found);
}

int	ft_unset(t_minishell **main, char **names, t_alloc **garbage)
{
	int	i;
	int	found;

	(void)garbage;
	i = 0;
	found = 0;
	while (names[i] != NULL)
	{
		if (unset_single_variable(&(*main)->envp, names[i]))
			found = 1;
		i++;
	}
	if (found >= 1)
		(*main)->code_status = 0;
	(*main)->code_status = 1;
	return ((*main)->code_status);
}
