/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 00:52:58 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_is_expand(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
				return (1);
			if (ft_isalnum(line[i + 1]) || line[i + 1] == UNDERSCORE)
				return (1);
		}
		i++;
	}
	return (0);
}

char	*heredoc_get_expand(char *line, char **envp, t_code *code, t_alloc **garbage)
{
	int		i;
	char	*name;
	char	*var;
	char	*new_line;

	i = 0;
	new_line = NULL;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] && line[i + 1] == '?')
			{
				i += 2;
				new_line = ft_strjoin(new_line, ft_itoa(code->code_status), garbage);
			}
			else if (line[i + 1] && (ft_isalnum(line[i + 1]) || line[i + 1] == UNDERSCORE))
			{
				name = get_env_var_name(line, &i, garbage);
				var = ft_getenv(&envp, name);
				if (var)
					new_line = ft_strjoin(new_line, var, garbage);
				new_line = ft_strjoin_char(new_line, 32, garbage);
			}
		}
		else
			new_line = ft_strjoin_char(new_line, line[i], garbage);
		i++;
	}
	return (new_line);
}

void	write_pipe(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	read_add(int fd, char *delimiter, t_code *code, char **envp, t_alloc **garbage)
{
	char	*line;
	
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (heredoc_is_expand(line))
			line = heredoc_get_expand(line, envp, code, garbage);
		write_pipe(fd, line);
	}
}

int		delimiter_size(t_command *command, int *i)
{
	int	j;
	int	size;

	j = 0;
	size = 0;
	while (command[*i + j].type == DB_LEFT_CHEV)
	{
		j += 2;
		size++;
	}
	return (size);
}

char	**get_delimiter(t_command *command, int *i, t_pipe *pipe, t_alloc **garbage)
{
	int		j;
	int		size;

	j = 0;
	size = delimiter_size(command, i);
	pipe->delimiter = garb_malloc(sizeof(char *), size + 1, garbage);
	while (command[*i].type == DB_LEFT_CHEV)
	{
		(*i)++;
		pipe->delimiter[j] = ft_strdup(command[*i].word, garbage);
		if (!pipe->delimiter[j])
			return (NULL);
		j++;
		(*i)++;
	}
	return (pipe->delimiter);
}

int	heredoc(t_pipe *pipes, t_command *command, int *i, char **cmd_args, char **envp, t_code *code, t_alloc **garbage)
{
	int				status;
	int				code_status;
	int				j;

	status = 0;
	code_status = 0;
	j = 0;
	pipes->delimiter = get_delimiter(command, i, pipes, garbage);
	while (pipes->delimiter[j])
	{	
		pipe(pipes->fd);
		read_add(pipes->fd[1], pipes->delimiter[j], code, envp, garbage);
		j++;
	}
	close(pipes->fd[1]);
	pipes->pid = fork();
	if (pipes->pid == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	else if (pipes->pid == 0)
	{
		if (cmd_args == NULL)
			cmd_args = create_cmd_args(command, i, garbage);
		heredoc_child(pipes, cmd_args, &envp, code, garbage);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipes->fd[0]);
		close(pipes->fd[1]);
		// printf("pid heredoc = %d\n", pipes->pid);
		waitpid(pipes->pid, &status, 0);
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





