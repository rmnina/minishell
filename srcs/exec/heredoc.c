/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/10 19:35:56 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_is_expand(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		//printf("heredoc_is_expand - Checking line[%d]: %c\n", i, line[i]); 
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1]) \
		|| line[i + 1] == UNDERSCORE))
			return (i);
		else if (line[i] == '$' && line[i + 1] && (line[i + 1] != '?'))
			return (i * -1);
		i++;
	}
	return (0);
}

char	*heredoc_get_expand(char *line, char **envp, t_alloc **garbage)
{
	int		index;
	char	*name;
	char	*var;

	index = heredoc_is_expand(line);
	name = get_env_var_name(line, &index, garbage);
	var = ft_getenv(&envp, name);
	if (!var)
		return (NULL);
	return (var);
}

t_line	*new_line(char *line, t_alloc **garbage)
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


void	read_add(int fd, const char *delimiter, t_alloc **garbage)
{
	char	*line;
	t_line	*node;
	t_line	*head;
	t_line	*tail;

	//printf("read_add - Starting with delimiter: %s\n", delimiter); //
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

int	heredoc(t_heredocNode *heredoclist, t_pipe *pipes, char **argv, char **envp, t_code *code, t_alloc **garbage)
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
	close(pipes->fd[1]);
	pid = fork();
	if (pid == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		heredoc_child(pipes, argv, &envp, code, garbage);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipes->fd[0]);
		close(pipes->fd[1]);
		// printf("pid heredoc = %d\n", pipes->pid);
		waitpid(pid, &status, 0);
		// printf("pid heredoc after wait = %d\n", pipes->pid);
		if (WIFEXITED(status))
		{
			code_status = WEXITSTATUS(status);
			if (code_status == SPECIAL_EXIT_CODE)
				exit(code_status);
			// exit(EXIT_SUCCESS);
		}
	}
	return (code_status);
}

// int heredoc(t_heredocNode *heredoclist, t_pipe *pipes, char **argv, char **envp, t_code *code, t_alloc **garbage)
// {

// 	pid_t			pid;
// 	int				status;
// 	int				code_status;
// 	t_heredocNode	*current;

// 	if (pipe(pipes->heredoc_fd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}

// 	current = heredoclist;
// 	while (current != NULL)
// 	{
// 		read_add(pipes->heredoc_fd[1], current->delimiter, garbage);
// 		current = current->next;
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pid == 0)
// 	{
// 		close(pipes->heredoc_fd[1]);
// 		heredoc_child(pipes, argv, &envp, code, garbage);
// 		exit(EXIT_SUCCESS);
// 	}
// 	else
// 	{
// 		close(pipes->heredoc_fd[0]);
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 		{
// 			code_status = WEXITSTATUS(status);
// 		}
// 	}

// 	return (code_status);
// }





