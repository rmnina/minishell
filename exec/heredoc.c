/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/28 14:35:46 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_line	*new_line(char *line, t_alloc *garbage)
{
	t_line	*node;

	node = malloc(sizeof(t_line));
	if (!node)
		return (NULL);
	node->line = ft_strdup(line, garbage);
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

void	write_pipe(int fd, t_line *head)
{
	t_line	*current;

	current = head;
	while (current)
	{
		write(fd, current->line, ft_strlen(current->line));
		write(fd, "\n", 1);
		current = current->next;
	}
}


void	read_add(int fd, const char *delimiter, t_alloc *garbage)
{
	char	*line;
	t_line	*node;
	t_line	*head;
	t_line	*tail;

	head = NULL;
	tail = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		node = new_line(line, garbage);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		free(line);
	}
	write_pipe(fd, head);
	free_line_nodes(head);
	close (fd);
}


int	heredoc(t_heredocNode *heredoclist, t_pipe *pipes, char **argv, char ***envp, t_alloc *garbage)
{
	pid_t			pid;
	int				status;
	int				code_status;
	t_heredocNode	*current;

	status = 0;
	code_status = 0;
	current = heredoclist;
	while (current != NULL)
	{
		pipe(pipes->fd);
		read_add(pipes->fd[1], current->delimiter, garbage);
		current = current->next;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		heredoc_child(pipes, argv, envp, garbage);
		exit (EXIT_SUCCESS);
	}
	else
	{
		close(pipes->fd[0]);
		close(pipes->fd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code_status = WEXITSTATUS(status);
	}
	return (code_status);
}