/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 18:49:49 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_output(char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

	fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (dup == -1)
		return (-1);
	return (dup);
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

int	is_output(t_minishell **main, int *i)
{
	if ((*main)->command[*i + 2].type && ((*main)->command[*i + 2].type == DB_RIGHT_CHEV \
	|| (*main)->command[*i + 2].type == RIGHT_CHEV))
		return (1);
	return (0);
}

int	get_all_redir(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_output(main, i))
	{
		while (is_output(main, i))
		{
			filename = ft_strdup((*main)->command[*i + 1].word, garbage);
			if (!filename)
				return (-1);
			(*main)->fd = open(filename, O_CREAT | O_WRONLY, 0644);
			close((*main)->fd);
			*i += 2;
		}
	}
	(*main)->pos = *i;
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (!filename)
		return (-1);
	if ((*main)->command[*i].type == RIGHT_CHEV)
		(*main)->fd = redir_output(filename);
	else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->fd = redir_append(filename);
	return (0);
}

int	child_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;
	
	filename = NULL;
	(*main)->fd = 0;
	if ((*main)->command[*i].type == DB_RIGHT_CHEV || \
		(*main)->command[*i].type == RIGHT_CHEV)
	{
		if (get_all_redir(main, i, garbage) == -1)
			exit(EXIT_FAILURE);
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
	{
		filename = ft_strdup((*main)->command[*i - 1].word, garbage);
		(*main)->fd = redir_input(filename);
	}
	if (execute_builtins(main, garbage) == -1)
		execute_non_builtin(main, garbage);
	return ((*main)->fd);
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
	pid_t	pid;
	int		status;

	(*main)->fd = 0;
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
		(*main)->fd = child_redirect(main, i, garbage);
		exit(0);
	}
	else if (pid > 0)
		parent_redirect((*main)->fd, pid, &status);
	*i += 2;
	return (1);
}
