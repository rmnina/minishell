/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 17:16:38 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/17 16:39:21 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_output(t_minishell **main, char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
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
	close((*main)->fd[0]);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	is_output(t_minishell **main, int *i)
{
	if ((*main)->command[*i + 2].type && \
	((*main)->command[*i + 2].type == DB_RIGHT_CHEV \
	|| (*main)->command[*i + 2].type == RIGHT_CHEV))
		return (1);
	return (0);
}

int	is_input(t_minishell **main, int *i)
{
	if ((*main)->command[*i + 2].type \
	&& ((*main)->command[*i + 2].type == LEFT_CHEV))
		return (1);
	return (0);
}
