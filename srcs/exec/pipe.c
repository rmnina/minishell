/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/16 02:58:51 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_pids(t_minishell **main, t_alloc **garbage)
{
	int	count;
	int	i;
	int	j;

	count = 0;
	i = 0;
	j = 0;
	while ((*main)->command[j].type)
	{
		if ((*main)->command[j].type == PIPE)
			count++;
		j++;
	}
	if (count == 0)
		return (0);
	(*main)->pid = garb_malloc(sizeof(pid_t), count + 1, garbage);
	if ((*main)->pid == NULL)
		return (-1);
	while (i < count)
	{
		(*main)->pid[i] = 1;
		i++;
	}
	(*main)->pid[i] = 0;
	return (count);
}

int	is_first_pipe(t_minishell **main, int *i)
{
	int	count;

	count = 0;
	while ((*main)->command[count].type != PIPE)
		count++;
	if (count == *i)
		return (1);
	return (0);
}

int	is_last_pipe(t_minishell **main, int *i)
{
	int	count;

	count = 0;
	while ((*main)->command[*i + count].type)
	{
		if ((*main)->command[*i + count].type == PIPE)
			return (0);
		count++;
	}
	return (1);
}

int	init_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	if (*i == 0 && (*main)->command[*i].type == DB_LEFT_CHEV)
	{
		(*main)->h_delimiter = get_delimiter(main, i, garbage);
		if ((*main)->h_delimiter == NULL)
			return (-1);
		return (1);
	}
	else if (*i > 0 && (*main)->command[*i - 1].type == PIPE \
	&& (*main)->command[*i].type == DB_LEFT_CHEV)
	{
		(*main)->h_delimiter = get_delimiter(main, i, garbage);
		if ((*main)->h_delimiter == NULL)
			return (-1);
		return (1);
	}
	return (0);
}

int	will_be_piped(t_minishell **main, int *i)
{
	int	j;

	j = 0;
	while ((*main)->command[*i + j].type && (*main)->command[*i + j].type != PIPE)
		j++;
	if ((*main)->command[*i].type == PIPE)
		return (1);
	return (0);
}

int	check_redir(t_minishell **main, int *i)
{
	int	j;

	j = 0;
	while ((*main)->command[*i + j].type && (*main)->command[*i + j].type != PIPE)
	{
		if ((*main)->command[*i + j].type >= RIGHT_CHEV \
		&& (*main)->command[*i + j].type <= DB_LEFT_CHEV)
			return (1);
		j++;
	}
	return (0);
}

int	execution(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->nb_cmd = 0;
	init_process_signal();
	if (init_heredoc(main, i, garbage) == -1)
		perror("heredoc delimiter");
	(*main)->total_cmd = init_pids(main, garbage);
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if (will_be_piped(main, i))
	{
		if (ft_pipex(main, i, garbage) == -1)
			return (-1);
	}
	if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= DB_RIGHT_CHEV)
	{
		if (ft_redirect(main, i, garbage) == -1)
			return (-1);
	}
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	return (1);
}

void	child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->com[0]);
	if (check_redir(main, i))
	{
		if (((*main)->redir = ft_redirect(main, i, garbage)) == -1)
			exit(EXIT_FAILURE);
	}
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	write((*main)->com[1], i, sizeof(*i));
	close((*main)->com[1]);
	exit(EXIT_SUCCESS);
}

int	first_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	int	status;

	status = 0;
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->pid[(*main)->nb_cmd] = fork();
	(*main)->nb_cmd++;
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
	{
		close((*main)->fd[0]);
		dup2((*main)->fd[1], STDIN_FILENO);
		close((*main)->fd[1]);
		child_process(main, i, garbage);
	}
	else
	{
		close((*main)->com[1]);
		close((*main)->fd[1]);
		(*main)->old_fd = dup((*main)->fd[0]);
		waitpid((*main)->pid[(*main)->nb_cmd - 1], &status, 0);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		
	}
	return (0);
}

int	middle_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	int	status;
	
	status = 0;
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->pid[(*main)->nb_cmd] = fork();
	(*main)->nb_cmd++;
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
	{
		close((*main)->fd[0]);
		dup2((*main)->old_fd, STDIN_FILENO);
		close((*main)->old_fd);
		dup2((*main)->fd[1], STDOUT_FILENO);
		close((*main)->fd[1]);
		child_process(main, i, garbage);
	}
	else
	{
		close((*main)->com[1]);
		close((*main)->old_fd);
		(*main)->old_fd = dup((*main)->fd[0]);
		close((*main)->fd[0]);
		close((*main)->fd[1]);
		waitpid((*main)->pid[(*main)->nb_cmd - 1], &status, 0);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		
	}
	return (0);
}

int	last_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	int	status;
	
	status = 0;
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->pid[(*main)->nb_cmd] = fork();
	(*main)->nb_cmd++;
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
	{
		close((*main)->fd[0]);
		dup2((*main)->old_fd, STDIN_FILENO);
		close((*main)->old_fd);
		child_process(main, i, garbage);
	}
	else
	{
		close((*main)->com[1]);
		waitpid((*main)->pid[(*main)->nb_cmd - 1], &status, 0);
		close((*main)->old_fd);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		
	}
	return (0);
}

int	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->com) == -1)
		return (-1);
	if (is_first_pipe(main, i))
		first_pipe(main, i, garbage);
	else if (is_last_pipe(main, i))
		last_pipe(main, i, garbage);
	else
		middle_pipe(main, i, garbage);
	if ((*main)->nb_cmd < (*main)->total_cmd && will_be_piped(main, i))
	{
		(*main)->cmd_args = create_cmd_args(main, i, garbage);
		ft_pipex(main, i, garbage);
	}
	return (0);
}

// void	handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
// {
// 	(*main)->redir = 0;
// 	(*main)->cmd_args = create_cmd_args(main, i, garbage);
// 	if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= DB_LEFT_CHEV)
// 		(*main)->redir = 1;
// }

// void	initialize_process(t_minishell **main, int *i)
// {
// 	(void)i;
// 	init_process_signal();
// 	pipe((*main)->fd);
// 	pipe((*main)->com);
// 	(*main)->pid = fork();
// 	if ((*main)->pid == -1)
// 	{
// 		perror("pid");
// 		exit(EXIT_FAILURE);
// 	}
// }

// void	execute_child_process(t_minishell **main, int *i, t_alloc **garbage)
// {
// 	if ((*main)->com[0] != -1)
// 		close((*main)->com[0]);
// 	if (((*main)->redir = ft_redirect(main, i, garbage)) == -1)
// 		exit(EXIT_FAILURE);
// 	if ((*main)->old_fd != -1)
// 	{
// 		dup2((*main)->old_fd, STDIN_FILENO);
// 		close((*main)->old_fd);
// 	}
// 	if ((*main)->fd[1] != -1)
// 	{
// 		dup2((*main)->fd[1], STDOUT_FILENO);
// 		close((*main)->fd[1]);
// 	}
// 	if ((*main)->com[1] != -1)
// 	{
// 		write((*main)->com[1], i, sizeof(*i));
// 		close((*main)->com[1]);
// 	}
// 	if (execute_builtins(main, garbage) == -1)
// 		execute_command(main, garbage);
// 	exit(EXIT_SUCCESS);
// }

// void	handle_parent_process(t_minishell **main, int *i, int *status)
// {
// 	if ((*main)->com[1] != -1)
// 		close((*main)->com[1]);
// 	if ((*main)->fd[1] != -1)
// 		close((*main)->fd[1]);
// 	if ((*main)->old_fd != -1)
// 		close((*main)->old_fd);
// 	(*main)->old_fd = (*main)->fd[0];
// 	waitpid((*main)->pid, status, 0);
// 	if ((*main)->com[0] != -1)
// 	{
// 		read((*main)->com[0], i, sizeof(*i));
// 		close((*main)->com[0]);
// 	}
// 	if (WIFEXITED(*status))
// 		(*main)->code_status = WEXITSTATUS(*status);
// }

// int 	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
// {
// 	int		status;

// 	status = 0;
// 	handle_command_args(main, i, garbage);
// 	if (is_builtin((*main)->command[0].word) && (*main)->command[*i].type == 0)
// 	{
// 		execute_builtins(main, garbage);
// 		return (0);
// 	}
// 	initialize_process(main, i);
// 	if ((*main)->pid == 0)
// 		execute_child_process(main, i, garbage);
// 	else
// 		handle_parent_process(main, i, &status);
// 	(*i)++;
// 	if ((*main)->command[*i].type != 0)
// 		ft_pipex(main, i, garbage);
// 	if ((*main)->old_fd != -1)
// 		close((*main)->old_fd);
// 	if ((*main)->infilefd != -1)
// 		close((*main)->infilefd);
// 	if ((*main)->outfilefd != -1)
// 		close((*main)->outfilefd);
// 	return (1);
// }