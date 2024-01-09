/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 11:09:38 by juandrie         ###   ########.fr       */
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

	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
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


t_heredocNode	*build_heredoclist(t_command *command, int *i, t_alloc *garbage)
{
	t_heredocNode	*head;
	t_heredocNode	*current;
	t_heredocNode	*new_node;

	head = NULL;
	current = NULL;
	new_node = garb_malloc(sizeof(t_heredocNode), 1, &garbage);
	if (!new_node)
		return (new_node);
	*i += 1;
	new_node->delimiter = command[*i].word;
	new_node->next = NULL;
	if (!head)
		head = new_node;
	else
		current->next = new_node;
	current = new_node;
	*i += 1;
	return (head);
}

int	init_redirection(t_command *command, int *i, char **cmd_args, char ***envp, t_code *code)
{
	char			*filename;
	int				fd;
	pid_t			pid;
	int				status;
	t_alloc			*son_garb;
	t_pipe			pipes;
	t_heredocNode	*heredoclist;

	son_garb = NULL;
	fd = 0;
	filename = NULL;
	printf("init_redirection: Initialisation de la redirection\n");
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		heredoclist = build_heredoclist(command, i, son_garb);
		if (heredoclist)
		{
			heredoc(heredoclist, &pipes, cmd_args, *envp, code, son_garb);
			exit(EXIT_SUCCESS);
		}
		if (command[*i].type == DB_RIGHT_CHEV || \
		command[*i].type == RIGHT_CHEV)
		{
			filename = ft_strdup(command[*i + 1].word, son_garb);
			if (!filename)
				return (-1);
			if (command[*i].type == RIGHT_CHEV)
				fd = redir_output(filename);
			else if (command[*i].type == DB_RIGHT_CHEV)
				fd = redir_append(filename);
		}
		else if (command[*i].type == LEFT_CHEV)
		{
			filename = command[*i - 1].word;
			fd = redir_input(filename);
		}
		if (execute_builtins(cmd_args, envp, code, son_garb) == -1)
			execute_non_builtin(envp, code, cmd_args, son_garb);
		free_garbage(&son_garb, 0);
		exit(0);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if (fd < -1)
				dup2(STDIN_FILENO, fd * -1);
			else if (fd > 0)
				dup2(STDOUT_FILENO, fd);
		}
	}
	*i += 2;
	return (1);
}
