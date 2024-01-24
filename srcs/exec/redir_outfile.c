/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_outfile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 22:46:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 18:29:19 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	process_output(t_minishell **main, int *i, char **filename)
{
	if ((*main)->command[*i].type == RIGHT_CHEV)
		(*main)->outfilefd = \
		open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->outfilefd = \
		open(*filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if ((*main)->outfilefd == -1)
		return (-1);
	close((*main)->outfilefd);
	return (1);
}

int	browse_outputs(t_minishell **main, int *i, char **filename, \
t_alloc **garbage)
{
	while (is_output(main, i))
	{
		*filename = ft_g_strdup((*main)->command[*i + 1].word, PARSING, garbage);
		if (!*filename)
			return (perror("filename alloc"), (*main)->code_status = 255, -1);
		if (process_output(main, i, filename) == -1)
			return (-1);
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

