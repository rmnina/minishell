/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/07 11:01:35 by juandrie         ###   ########.fr       */
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

int	heredoc(const char *delimiter, t_pipe *pipes, char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	int		code_status;

	status = 0;
	code_status = 0;
	printf("heredoc started with delimiter: %s\n", delimiter);
	if (pipe(pipes->pipefd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	printf("heredoc pipe and fork created\n");
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		heredoc_child(pipes, argv, envp);
	else
	{
		read_add(pipes->pipefd[1], delimiter);
		close(pipes->pipefd[0]);
		close(pipes->pipefd[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code_status = WEXITSTATUS(status);
		printf("heredoc read and write completed\n");
	}
	return (code_status);
}
