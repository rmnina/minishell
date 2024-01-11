/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 16:39:07 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_output(char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR \
	| S_IRGRP | S_IWGRP | S_IWUSR);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	redir_append(char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR \
	| S_IRGRP | S_IWGRP | S_IWUSR);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (dup == -1)
		return (-1);
	return (fd);
}

int	redir_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (-1);
	return (fd *= -1);
}

int	child_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;
	int		fd;

	fd = 0;
	filename = NULL;
	if ((*main)->command[*i].type == DB_RIGHT_CHEV || \
		(*main)->command[*i].type == RIGHT_CHEV)
		{
			filename = ft_strdup((*main)->command[*i + 1].word, garbage);
			if (!filename)
				return (-1);
			if ((*main)->command[*i].type == RIGHT_CHEV)
				fd = redir_output(filename);
			else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
				fd = redir_append(filename);
		}
		else if ((*main)->command[*i].type == LEFT_CHEV)
		{
			filename = (*main)->command[*i - 1].word;
			fd = redir_input(filename);
		}
		if (execute_builtins(main, garbage) == -1)
			execute_non_builtin(main, garbage);
		return (fd);
}

void	parent_redirect(int fd, pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status))
	{
		if (fd < -1)
			dup2(STDIN_FILENO, fd * -1);
		else if (fd > 0)
			dup2(STDOUT_FILENO, fd);
	}
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	int		fd;
	pid_t	pid;
	int		status;

	fd = 0;
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		ft_heredoc(main, i, garbage);
		return (0);
	}
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		fd = child_redirect(main, i, garbage);
		exit(0);
	}
	else if (pid > 0)
		parent_redirect(fd, pid, &status);
	*i += 2;
	return (1);
}
