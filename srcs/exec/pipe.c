/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/17 21:12:15 by jdufour          ###   ########.fr       */
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
	if ((*main)->command[*i + j].type == PIPE)
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
			return (j);
		j++;
	}
	return (-1);
}

void	wait_pids(t_minishell **main)
{
	int	j;
	int	status;
	
	j = 0;
	status = 0;
	while ((*main)->pid[j] != -2)
	{
		if (waitpid((*main)->pid[j], &status, 0) == -1)
		{
			(*main)->code_status = 255;
			perror("waitpid");
			exit(255);
		}
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		j++;
	}
}

void	child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->com[0]);
	if (check_redir(main, i))
	{
		if (((*main)->redir = ft_redirect(main, i, garbage)) == -1)
		{
			(*main)->code_status = 1;
			exit(EXIT_FAILURE);
		}
	}
	write((*main)->com[1], i, sizeof(*i));
	close((*main)->com[1]);
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	exit(EXIT_SUCCESS);
}

int first_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->fd) == -1)
        return (-1);
    (*main)->nb_cmd++;
    (*main)->pid[(*main)->nb_cmd - 1] = fork();
    if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
        return (-1);
    if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
    {
		if ((*main)->command[*i].type == PIPE)
			dup2((*main)->fd[1], STDOUT_FILENO);
		close((*main)->fd[1]);
        child_process(main, i, garbage);
    }
    else
    {
		close((*main)->com[1]);
        close((*main)->fd[1]);
		(*main)->old_fd = dup((*main)->fd[0]);
		if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
		if ((*main)->infilefd > 0)
			close ((*main)->infilefd);
		if ((*main)->outfilefd > 0)
			close ((*main)->outfilefd);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
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
		dup2((*main)->fd[1], STDOUT_FILENO);
       	dup2((*main)->old_fd, STDIN_FILENO);
        child_process(main, i, garbage);
    }
    else
    {
		close((*main)->com[1]);
        close((*main)->fd[1]);
		close((*main)->old_fd);
        (*main)->old_fd = dup((*main)->fd[0]);
		close((*main)->fd[0]);
		if ((*main)->infilefd > 0)
			close ((*main)->infilefd);
		if ((*main)->outfilefd > 0)
			close ((*main)->outfilefd);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
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
		close((*main)->fd[1]);
		if (dup2((*main)->old_fd, STDIN_FILENO) < 0)
		{
			(*main)->code_status = 1, 
			(exit(EXIT_FAILURE));
		}
        close((*main)->old_fd);
        child_process(main, i, garbage);
    }
    else
    {
        close((*main)->com[1]);
        close((*main)->fd[1]);
		close((*main)->old_fd);
		if (!(*main)->command[*i].type)
			close((*main)->fd[0]);
		if ((*main)->infilefd > 0)
			close ((*main)->infilefd);
		if ((*main)->outfilefd > 0)
			close ((*main)->outfilefd);
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
    }
    return 0;
}

int	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	init_process_signal();
	if (pipe((*main)->com) == -1)
		return (-1);
	while ((*main)->command[*i].type && (*main)->code_status != 1)
	{
		if (is_first_pipe(main, i))
			first_pipe(main, i, garbage);
		else
			middle_pipe(main, i, garbage);
		(*i)++;
		// dprintf(2, "cmd args avant update = %s\n", (*main)->cmd_args[0]);
		(*main)->cmd_args = create_cmd_args(main, i, garbage);
		// dprintf(2, "cmd args APRES update = %s\n", (*main)->cmd_args[0]);
	}
	last_pipe(main, i, garbage);
	wait_pids(main);
	return (0);
}
