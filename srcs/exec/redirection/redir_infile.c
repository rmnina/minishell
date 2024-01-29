/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_infile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 22:46:17 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 14:27:27 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	browse_inputs(t_minishell **main, int *i, \
char **filename, t_alloc **garbage)
{
	while (is_input(main, i))
	{
		*filename = ft_g_strdup((*main)->command[*i + 1].word, PARSING, garbage);
		if (!*filename)
			return (perror("filename alloc"), (*main)->code_status = 255, -1);
		(*main)->infilefd = open(*filename, O_RDONLY, 0644);
		if ((*main)->infilefd == -1)
			return (-1);
		close((*main)->infilefd);
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

char	*get_last_in_filename(t_minishell **main, int *i, t_alloc **garbage)
{
	int		j;
	char	*filename;

	j = *i;
	filename = NULL;
	while ((*main)->command[j].type && (*main)->command[j].type != PIPE)
		j++;
	while (j > 0 && (*main)->command[j].type != LEFT_CHEV)
		j--;
	return (filename = \
	ft_g_strdup((*main)->command[j + 1].word, EXEC, garbage));
}

int	check_outputs(t_minishell **main, int *i, t_alloc **garbage)
{
	check_next_args(main, i, garbage);
	if (is_output(main, i))
	{
		(*i) += 2;
		if (get_all_outputs(main, i, garbage) == -1)
			return (-1);
	}
	return (0);
}

int	get_input_files(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_input(main, i))
	{
		if (browse_inputs(main, i, &filename, garbage) == -1)
			return (-1);
	}
	filename = ft_g_strdup((*main)->command[*i + 1].word, PARSING, garbage);
	if (!filename)
		return (-1);
	(*main)->infilefd = open(filename, O_RDONLY, 0644);
	if ((*main)->infilefd == -1)
		return (-1);
	close((*main)->infilefd);
	if (check_outputs(main, i, garbage) == -1)
		return (-1);
	filename = get_last_in_filename(main, i, garbage);
	(*main)->infilefd = redir_input(main, filename);
	if ((*main)->infilefd == -1)
		return (-1);
	return (1);
}

int	get_all_inputs(t_minishell **main, int *i, t_alloc **garbage)
{
	if (get_input_files(main, i, garbage) == -1)
		return (-1);
	return (1);
}
