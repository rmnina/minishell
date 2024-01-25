/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 19:10:44 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 22:34:50 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	check_next_args(t_minishell **main, int *i, t_alloc **garbage)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	if ((*main)->command[*i + 2].type && (*main)->command[*i + 2].type == WORD)
	{
		(*i) += 2;
		while ((*main)->command[*i + j].type && \
		(*main)->command[*i + j].type == WORD)
			j++;
	}
	while (k < j)
	{
		(*main)->cmd_args = ft_strjoin_args(main, i, EXEC, garbage);
		(*i)++;
		k++;
	}
}

int	check_next_redir(t_minishell **main, int *i)
{
	int	j;

	j = 1;
	while ((*main)->command[*i + j].type && \
	(*main)->command[*i + j].type != PIPE)
	{
		if ((*main)->command[*i + j].type >= LEFT_CHEV \
		&& (*main)->command[*i + j].type <= DB_LEFT_CHEV)
			return (j);
		j++;
	}
	return (0);
}

int	handle_output_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	if (get_all_outputs(main, i, garbage) == -1)
	{
		(*main)->code_status = 1;
		write(2, "error : file could not be opened\n", 34);
		return (-1);
	}
	return (0);
}

int	handle_input_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	if (get_all_inputs(main, i, garbage) == -1)
	{
		(*main)->code_status = 1;
		write(2, "error : file could not be opened\n", 34);
		return (-1);
	}
	return (0);
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->infilefd = -2;
	(*main)->outfilefd = -2;
	if ((*main)->command[*i].type == DB_RIGHT_CHEV \
	|| (*main)->command[*i].type == RIGHT_CHEV)
	{
		if (handle_output_redirect(main, i, garbage) == -1)
			return (-1);
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
	{
		if (handle_input_redirect(main, i, garbage) == -1)
			return (-1);
	}
	if (check_redir(main, i) == 0)
		*i += 2;
	else
		*i += 1;
	return (1);
}
