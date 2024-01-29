/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_outfile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:23:32 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 14:22:10 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_inputs(t_minishell **main, int *i, char **filename, t_alloc **garbage)
{
	*filename = ft_g_strdup((*main)->command[*i + 1].word, EXEC, garbage);
	if (!*filename)
		return (-1);
	if ((*main)->command[*i].type == RIGHT_CHEV)
		(*main)->outfilefd = \
		open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->outfilefd = \
		open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (access(*filename, X_OK) != 0)
		return (-1);
	close((*main)->outfilefd);
	*i += 2;
	if (get_all_inputs(main, i, garbage) == -1)
		return (-1);
	return (0);
}

int	handle_output(t_minishell **main, int *i, char **filename, t_alloc **garbage)
{
	if (is_output(main, i))
	{
		if (browse_outputs(main, i, filename, garbage) == -1)
			return (-1);
	}
	check_next_args(main, i, garbage);
	return (0);
}

int get_all_outputs(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;
	int	type;

	filename = NULL;
	if (handle_output(main, i, &filename, garbage) == -1)
		return (-1);
	if (is_input(main, i))
	{
		if (check_inputs(main, i, &filename, garbage) == -1)
			return (-1);
	}
	filename = get_last_out_filename(main, i, garbage);
	type = get_last_out_type(main, i);
	if (type == RIGHT_CHEV)
		(*main)->outfilefd = redir_output(main, filename);
	else if (type == DB_RIGHT_CHEV)
		(*main)->outfilefd = redir_append(main, filename);
	if ((*main)->outfilefd == -1)
		return (-1);
	return (0);
}