/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/28 19:12:47 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_line	*new_line(char *line)
{
	t_line	*node;

	node = malloc(sizeof(t_line));
	if (!node)
		return (NULL);
	node->line = ft_strdup(line);
	node->next = NULL;
	return (node);
}

void	free_line_nodes(t_line *head)
{
	t_line	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->line);
		free(temp);
	}
}

pid_t	heredoc_pipe(t_pipe *pipes)
{
	pid_t	pid;

	if (pipe(pipes->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	heredoc_child(t_pipe *pipes, char **argv, char **envp)
{
	char	*path;
	char	*new_argv[2];

	close(pipes->pipefd[1]);
	if (dup2(pipes->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipes->pipefd[0]);
	path = find_command_path(argv[0]);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(argv[0]);
	new_argv[1] = NULL;
	execve(path, new_argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	write_pipe(int pipefd, t_line *head)
{
	t_line	*current;

	current = head;
	while (current)
	{
		write(pipefd, current->line, strlen(current->line));
		write(pipefd, "\n", 1);
		current = current->next;
	}
}


void	read_add(int pipefd, const char *delimiter)
{
	char	*line;
	t_line	*node;
	t_line	*head;
	t_line	*tail;

	head = NULL;
	tail = NULL;
	while (1)
	{
		line = readline(NULL);
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		node = new_line(line);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		free(line);
	}
	write_pipe(pipefd, head);
	free_line_nodes(head);
}

void	here_doc(const char *delimiter, t_pipe *pipes, char **argv, char **envp)
{
	pid_t	pid;

	if (pipe(pipes->pipefd) == -1)
	{
		perror("erreur de creation du pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror ("Erreur lors du fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		heredoc_child(pipes, argv, envp);
	else
	{
		read_add(pipes->pipefd[1], delimiter);
		close(pipes->pipefd[0]);
		close(pipes->pipefd[1]);
		waitpid(pid, NULL, 0);
	}
}
