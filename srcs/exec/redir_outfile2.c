/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_outfile2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:23:32 by juandrie          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/01/24 18:26:51 by juandrie         ###   ########.fr       */
=======
/*   Updated: 2024/01/24 18:24:44 by juandrie         ###   ########.fr       */
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

<<<<<<< HEAD
int	handle_output(t_minishell **main, int *i, char **filename, \
t_alloc **garbage)
=======
int	handle_output(t_minishell **main, int *i, char **filename, t_alloc **garbage)
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
{
	if (is_output(main, i))
	{
		if (browse_outputs(main, i, filename, garbage) == -1)
			return (-1);
	}
	check_next_args(main, i, garbage);
	return (0);
}

<<<<<<< HEAD
int	finalize_out(t_minishell **main, int *i, char **filename, t_alloc **garbage)
=======
int finalize_out(t_minishell **main, int *i, char **filename, t_alloc **garbage)
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
{
	int	type;

	if (is_input(main, i))
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
		close((*main)->outfilefd);
		*i += 2;
		if (get_all_inputs(main, i, garbage) == -1)
			return (-1);
	}
	*filename = get_last_out_filename(main, i, garbage);
	type = get_last_out_type(main, i);
	if (type == RIGHT_CHEV)
		(*main)->outfilefd = redir_output(main, *filename);
	else if (type == DB_RIGHT_CHEV)
		(*main)->outfilefd = redir_append(main, *filename);
	return (0);
}

<<<<<<< HEAD
int	get_all_outputs(t_minishell **main, int *i, t_alloc **garbage)
=======

int get_all_outputs(t_minishell **main, int *i, t_alloc **garbage)
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
{
	char	*filename;

	filename = NULL;
	if (handle_output(main, i, &filename, garbage) == -1)
		return (-1);
	if (finalize_out(main, i, &filename, garbage) == -1)
		return (-1);
	return (0);
<<<<<<< HEAD
}
=======
}
>>>>>>> bbc7f77432f013053cd10998c7d7d565ca051551
