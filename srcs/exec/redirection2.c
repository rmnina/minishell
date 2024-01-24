/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 19:10:44 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 19:12:01 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	handle_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	int	ret;

	ret = 0;
	if ((*main)->command[*i].type == DB_RIGHT_CHEV \
	|| (*main)->command[*i].type == RIGHT_CHEV)
	{
		ret = handle_output_redirect(main, i, garbage);
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
	{
		ret = handle_input_redirect(main, i, garbage);
	}
	if (check_redir(main, i) == 0)
		*i += 2;
	else
		*i += 1;
	return (ret);
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->infilefd = -2;
	(*main)->outfilefd = -2;
	if (handle_redirect(main, i, garbage) == -1)
		return (-1);
	return (1);
}
