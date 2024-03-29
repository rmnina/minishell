/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 17:16:38 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 14:12:06 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	redir_output(t_minishell **main, char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if ((*main)->fd[1] != -1)
		close((*main)->fd[1]);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	redir_append(t_minishell **main, char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if ((*main)->fd[1] != -1)
		close((*main)->fd[1]);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	redir_input(t_minishell **main, char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_RDONLY, 0644);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDIN_FILENO);
	close(fd);
	if ((*main)->fd[0] != -1)
		close((*main)->fd[0]);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	is_output(t_minishell **main, int *i)
{
	int	j;

	j = check_next_redir(main, i);
	if (j == 0)
		return (0);
	if ((*main)->command[*i + j].type && \
	((*main)->command[*i + j].type == DB_RIGHT_CHEV \
	|| (*main)->command[*i + j].type == RIGHT_CHEV))
		return (1);
	return (0);
}

int	is_input(t_minishell **main, int *i)
{
	int	j;

	j = check_next_redir(main, i);
	if (j == 0)
		return (0);
	if ((*main)->command[*i + j].type \
	&& ((*main)->command[*i + j].type == LEFT_CHEV))
		return (1);
	return (0);
}
