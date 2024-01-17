/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/17 16:12:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_pids(t_minishell **main, t_alloc **garbage)
{
	int	count;
	int	i;
	int	j;

	count = 1;
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
	(*main)->pid[i] = -2;
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
		if ((*main)->command[*i + j].type >= LEFT_CHEV \
		&& (*main)->command[*i + j].type <= DB_LEFT_CHEV)
			return (1);
		j++;
	}
	return (0);
}

void	wait_pids(t_minishell **main, int *i)
{
	int	j;
	int	status;
	
	j = 0;
	status = 0;
	while ((*main)->pid[j] != -2)
	{
		close((*main)->com[1]);
		close((*main)->fd[1]);
		if (waitpid((*main)->pid[j], &status, 0) == -1)
		{
			(*main)->code_status = 255;
			perror("waitpid");
			exit(255);
		}
		// if ((*main)->old_fd != -1)
		// 	close ((*main)->old_fd);
		// if (!will_be_piped(main, i))
		// 	close((*main)->fd[0]);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		j++;
	}
	read((*main)->com[0], i, sizeof(*i));
	close((*main)->com[0]);
}

void	child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->com[0]);
	if (check_redir(main, i))
	{
		if (((*main)->redir = ft_redirect(main, i, garbage)) == -1)
			exit(EXIT_FAILURE);
	}
	write((*main)->com[1], i, sizeof(*i));
	close((*main)->com[1]);
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	exit(EXIT_SUCCESS);
}

int first_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
    dprintf(2, "je suis bien rentré ici\n");
	if (pipe((*main)->fd) == -1)
        return (-1);
    (*main)->nb_cmd++;
    (*main)->pid[(*main)->nb_cmd - 1] = fork();
    if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
        return (-1);
    if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
    {
		close((*main)->fd[0]);
		if ((*main)->command[*i].type == PIPE && dup2((*main)->fd[1], STDOUT_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
		close((*main)->fd[1]);
		// dup2((*main)->fd[0], STDIN_FILENO);
        child_process(main, i, garbage);
    }
    else
    {
		close((*main)->com[1]);
        close((*main)->fd[1]);
		(*main)->old_fd = dup(STDIN_FILENO);
		// dup2((*main)->fd[0], STDIN_FILENO);
		if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
    }
    return (0);
}

int middle_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
    if (pipe((*main)->fd) == -1)
        return (-1);
    (*main)->nb_cmd++;
    (*main)->pid[(*main)->nb_cmd - 1] = fork();
    if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
        return (-1);
    if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
    {
        close((*main)->fd[0]);
        if (dup2((*main)->old_fd, STDIN_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
        // close((*main)->old_fd);
        if (dup2((*main)->fd[1], STDOUT_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
        // close((*main)->fd[1]);
        child_process(main, i, garbage);
    }
    else
    {
        close((*main)->old_fd);
        (*main)->old_fd = dup((*main)->fd[0]);
	    if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
        // close((*main)->fd[1]);
		// close((*main)->com[1]);
    }
    return 0;
}

int last_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
    if (pipe((*main)->fd) == -1)
        return (-1);
    (*main)->nb_cmd++;
    (*main)->pid[(*main)->nb_cmd - 1] = fork();
    if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
        return (-1);
    if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
    {
    	close((*main)->fd[0]);
		if ((*main)->command[*i].type == PIPE && dup2((*main)->fd[1], STDOUT_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
	    else 
			close((*main)->fd[1]);
		if (dup2((*main)->old_fd, STDIN_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
        // close((*main)->old_fd);
        child_process(main, i, garbage);
    }
    else
    {
        close((*main)->old_fd);
		if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
    }
    return 0;
}

int	pipelines(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->fd) == -1)
        return (-1);
    (*main)->nb_cmd++;
    (*main)->pid[(*main)->nb_cmd - 1] = fork();
    if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
        return (-1);
    if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
    {
		close((*main)->fd[0]);
		if ((*main)->command[*i].type == PIPE && dup2((*main)->fd[1], STDOUT_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
		if ((*main)->old_fd != -1 && dup2((*main)->old_fd, STDIN_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
		child_process(main, i, garbage);
	}
	else
	{
		if (will_be_piped(main, i))
			(*main)->old_fd = dup((*main)->fd[0]);
		close((*main)->com[1]);
		close((*main)->fd[1]);
		if (waitpid((*main)->pid[(*main)->nb_cmd - 1], &(*main)->status, 0) == -1)
		{
			(*main)->code_status = 255;
			perror("waitpid");
			exit(255);
		}
		if ((*main)->old_fd == -1)
			(*main)->old_fd = dup(STDIN_FILENO);
		else if ((*main)->old_fd != -1 && !will_be_piped(main, i))
			close ((*main)->old_fd);
		if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
		if (WIFEXITED((*main)->status))
			(*main)->code_status = WEXITSTATUS((*main)->status);
	}
	return (0);
}

void	restore_fds(t_minishell **main)
{
	if ((*main)->fd[0] > 0)
		dup2(STDIN_FILENO, ((*main)->fd[0]));
	if ((*main)->fd[1] > 0)
		dup2(STDOUT_FILENO, ((*main)->fd[1]));;
	if ((*main)->old_fd > 0)
		close((*main)->old_fd);
}

int	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	// init_process_signal();
	if (pipe((*main)->com) == -1)
		return (-1);
	while ((*main)->command[*i].type)
	{
		if (is_first_pipe(main, i))
			first_pipe(main, i, garbage);
		else if (is_last_pipe(main, i))
			last_pipe(main, i, garbage);
		else
			middle_pipe(main, i, garbage);
		(*i)++;
		(*main)->cmd_args = create_cmd_args(main, i, garbage);
		if (!(*main)->command[*i].type)
			last_pipe(main, i, garbage);
	}
	wait_pids(main, i);
	// if (execute_builtins(main, garbage) == -1)
	// 	execute_non_builtin(main, garbage);
	// if ((*main)->nb_cmd == (*main)->total_cmd)
	// {
	// 	last_pipe(main, i, garbage);
	// 	dup2((*main)->old_fd, STDIN_FILENO);
	// }
	// if ((*main)->nb_cmd < (*main)->total_cmd && will_be_piped(main, i))
	// {
	// 	(*i)++;
	// 	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	// 	ft_pipex(main, i, garbage);
	// }
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