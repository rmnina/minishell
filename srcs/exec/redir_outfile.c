/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_outfile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 22:46:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 18:13:55 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	browse_outputs(t_minishell **main, int *i, char **filename, t_alloc **garbage)
{
	while (is_output(main, i))
	{
		*filename = ft_g_strdup((*main)->command[*i + 1].word, PARSING, garbage);
		if (!*filename)
			return (perror("filename alloc"), (*main)->code_status = 255, -1);
		if ((*main)->command[*i].type == RIGHT_CHEV)
		{
			if (((*main)->outfilefd = \
			open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1)
				return (-1);
		}
		else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		{
			if (((*main)->outfilefd = \
			open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644)) == -1)
				return (-1);
		}
		close((*main)->outfilefd);
		if (check_next_redir(main, i) != 2)
		{
			*i += 2;
			while (check_redir(main, i) != 0)
			{
				(*main)->cmd_args = ft_strjoin_args(main, i, PARSING, garbage);
				(*i)++;
			}
		}
		else
			*i += 2;
	}
	return (1);
}

char	*get_last_out_filename(t_minishell **main, int *i, t_alloc **garbage)
{
	int		j;
	char	*filename;

	j = *i;
	filename = NULL;
	while ((*main)->command[j].type && (*main)->command[j].type != PIPE)
		j++;
	while (j > 0 && (*main)->command[j].type != RIGHT_CHEV \
	&& (*main)->command[j].type != DB_RIGHT_CHEV)
		j--;
	if ((*main)->command[j + 1].type)
		filename = ft_g_strdup((*main)->command[j + 1].word, PARSING, garbage);
	return (filename);
}

int	get_last_out_type(t_minishell **main, int *i)
{
	int		j;
	int		type;

	j = *i;
	type = 0;
	while ((*main)->command[j].type && (*main)->command[j].type != PIPE)
		j++;
	while (j > 0 && (*main)->command[j].type != RIGHT_CHEV \
	&& (*main)->command[j].type != DB_RIGHT_CHEV)
		j--;
	return (type = (*main)->command[j].type);
}

int	get_all_outputs(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;
	int		type;

	filename = NULL;
	if (is_output(main, i))
	{
		if (browse_outputs(main, i, &filename, garbage) == -1)
			return (-1);
	}
	check_next_args(main, i, garbage);
	if (is_input(main, i))
	{
		filename = ft_g_strdup((*main)->command[*i + 1].word, PARSING, garbage);
		if (!filename)
			return (-1);
		if ((*main)->command[*i].type == RIGHT_CHEV)
			(*main)->outfilefd = \
			open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
			(*main)->outfilefd = \
			open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		close ((*main)->outfilefd);
		(*i) += 2;
		if (get_all_inputs(main, i, garbage) == -1)
			return (-1);
	}
	filename = get_last_out_filename(main, i, garbage);
	type = get_last_out_type(main, i);
	if (type == RIGHT_CHEV)
		(*main)->outfilefd = redir_output(main, filename);
	else if (type == DB_RIGHT_CHEV)
		(*main)->outfilefd = redir_append(main, filename);
	return (0);
}
