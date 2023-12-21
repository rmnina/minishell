/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/21 15:07:37 by juandrie         ###   ########.fr       */
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
	printf("read_add: Entrée\n"); 
	while (1)
	{
		line = readline("heredoc> ");
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
	printf("read_add: Sortie\n");
}

int	heredoc(const char *delimiter, t_pipe *pipes, char **argv, char **envp, t_alloc *garbage)
{
	pid_t	pid;
	int		status;
	int		code_status;

	status = 0;
	code_status = 0;
	if (pipe(pipes->fd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		heredoc_child(pipes, argv, envp, garbage);
	else
	{
		//printf("heredoc: Dans le processus parent, avant read_add\n"); // Debug
		read_add(pipes->fd[1], delimiter, garbage);
		//printf("heredoc: Dans le processus parent, après read_add\n"); // Debug
		close(pipes->fd[0]);
		close(pipes->fd[1]);
		waitpid(pid, &status, 0);
		//printf("heredoc: Processus parent, après waitpid\n");
		if (WIFEXITED(status))
			code_status = WEXITSTATUS(status);
	}
	//printf("heredoc: Sortie\n"); 
	return (code_status);
}
