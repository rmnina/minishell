/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids_and_fds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 22:11:01 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/25 02:51:56 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	init_pids(t_minishell **main, t_alloc **garbage)
{
	int	count;
	int	i;
	int	j;

	count = 1;
	i = 0;
	j = 0;
	while ((*main)->command[j].type)
	{
		if ((*main)->command[j].type == PIPE)
			count++;
		j++;
	}
	if (count == 0)
		return (0);
	(*main)->pid = garb_malloc(sizeof(pid_t), count + 1, EXEC, garbage);
	if ((*main)->pid == NULL)
		return (-1);
	while (i < count)
	{
		(*main)->pid[i] = 1;
		i++;
	}
	(*main)->pid[i] = -2;
	return (count);
}

void	wait_pids(t_minishell **main)
{
	int	j;
	int	status;

	j = 0;
	status = 0;
	while ((*main)->pid[j] != -2)
	{
		if (waitpid((*main)->pid[j], &status, 0) == -1)
		{
			(*main)->code_status = 255;
			perror("waitpid");
			exit(255);
		}
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		j++;
	}
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