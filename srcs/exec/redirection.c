/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/17 21:29:59 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_next_args(t_minishell **main, int *i, t_alloc **garbage)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	if ((*main)->command[*i + 2].type && (*main)->command[*i + 2].type == WORD)
	{
    	(*i) += 2;
		while ((*main)->command[*i + j].type && (*main)->command[*i + j].type == WORD)
			j++;
	}
	while (k < j)
	{
		(*main)->cmd_args = ft_strjoin_args(main, i, garbage);
		(*i)++;
		k++;
	}
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
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (!filename)
		return (-1);
	type = (*main)->command[*i].type;
	check_next_args(main, i, garbage);
	if (is_input(main, i))
	{
		(*main)->outfilefd = open(filename, O_CREAT | O_WRONLY, 0644);
		close((*main)->outfilefd);
		get_all_inputs(main, i, garbage);
	}
	if (type == RIGHT_CHEV)
		(*main)->outfilefd = redir_output(main, filename);
	else if (type == DB_RIGHT_CHEV)
		(*main)->outfilefd = redir_append(main, filename);
	return (0);
}

int	get_all_inputs(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_input(main, i))
	{
		if (browse_inputs(main, i, &filename, garbage) == -1)
			return (-1);
	}
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (!filename)
		return (-1);
	if (open(filename, O_RDONLY, 0644) == -1)
		return (-1);
	check_next_args(main, i, garbage);
	if (is_output(main, i))
		get_all_outputs(main, i, garbage);
	if (((*main)->infilefd = redir_input(main, filename)) == -1)
		return (-1);
	return (1);
}

void	handle_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	if ((*main)->command[*i].type == DB_RIGHT_CHEV || \
		(*main)->command[*i].type == RIGHT_CHEV)
	{
		if (get_all_outputs(main, i, garbage) == -1)
		{
			(*main)->code_status = 1;
			write(2, "error : file could not be opened\n", 34);
			exit(EXIT_FAILURE);
		}
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
	{
		if (get_all_inputs(main, i, garbage) == -1)
		{
			(*main)->code_status = 1;
			write(2, "error : file could not be opened\n", 34);
			exit(EXIT_FAILURE);
		}
	}
	if (check_redir(main, i) == 0)
		*i += 2;
	else
		*i += 1;
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->infilefd = -2;
	(*main)->outfilefd = -2;
	
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		ft_heredoc(main, i, garbage);
		return (0);
	}
	handle_redirect(main, i, garbage);
	if ((*main)->infilefd == -1 || (*main)->outfilefd == -1)
	{
		write(2, "error : file could not be opened\n", 34);
		return(-1);
	}
	return (1);
}
