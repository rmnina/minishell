/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:50:11 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 19:05:33 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->fd[0]);
	dup2((*main)->fd[1], STDOUT_FILENO);
	close((*main)->fd[1]);
	if (!(*main)->heredoc)
		dup2((*main)->old_fd, STDIN_FILENO);
	if ((*main)->old_fd > 0)
		close((*main)->old_fd);
	child_process(main, i, garbage);
}

void	handle_parent_process(t_minishell **main)
{
	close((*main)->fd[1]);
	if ((*main)->old_fd > 0)
		close((*main)->old_fd);
	(*main)->old_fd = dup((*main)->fd[0]);
	close((*main)->fd[0]);
}

int	middle_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->nb_cmd++;
	(*main)->pid[(*main)->nb_cmd - 1] = fork();
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
		handle_child_process(main, i, garbage);
	else
		handle_parent_process(main);
	return (0);
}

int	last_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->nb_cmd++;
	(*main)->pid[(*main)->nb_cmd - 1] = fork();
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
	{
		close((*main)->fd[0]);
		close((*main)->fd[1]);
		if (!(*main)->heredoc)
			dup2((*main)->old_fd, STDIN_FILENO);
		if ((*main)->old_fd > 0)
			close((*main)->old_fd);
		child_process(main, i, garbage);
	}
	else
	{
		close((*main)->fd[0]);
		close((*main)->fd[1]);
		if ((*main)->old_fd > 0)
			close((*main)->old_fd);
	}
	return (0);
}

void	restore_fds(t_minishell **main)
{
	if ((*main)->fd[0] > 0)
		close((*main)->fd[0]);
	if ((*main)->fd[1] > 0)
		close((*main)->fd[1]);
	if ((*main)->infilefd > 0)
		close((*main)->infilefd);
	if ((*main)->outfilefd > 0)
		close((*main)->outfilefd);
	if ((*main)->old_fd > 0)
		close((*main)->old_fd);
}
