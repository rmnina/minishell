/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/13 01:16:14 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redir_output(char *filename)
{
	int	fd;
	int	dup;
	
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	redir_append(char *filename)
{
	int	fd;

	fd = open(filename, O_APPEND);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) == -1)
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

int	init_redirection(t_command *command, int *i, t_alloc *garbage)
{
	char	*filename;
	int		fd;
	
	if (command[*i].type == DB_LEFT_CHEV)
		return (-1);
		//heredoc;
	else
	{
		filename = ft_strdup(command[*i + 1].word, garbage);
		printf("filename : %s\n", filename);
		if (!filename)
			return (-1);
		if (command[*i].type == LEFT_CHEV)
			fd = redir_input(filename);
		else if (command[*i].type == RIGHT_CHEV)
			fd = redir_output(filename);
		else if (command[*i].type == DB_RIGHT_CHEV)
			fd = redir_append(filename);
	}
	*i += 2;
	return (fd);
}