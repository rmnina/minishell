/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 22:16:20 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void handle_command_args(t_command *command, char ***cmd_args, int *i, t_alloc **garbage)
{
    if (command[*i].type == WORD)
        *cmd_args = create_cmd_args(command, i, garbage);
}

void initialize_process(pid_t *pid, t_pipe *pipes, t_command *command, int *i)
{
    if (command[*i].type == PIPE || *i > 0)
        pipe(pipes->fd);

    *pid = fork();

    if (*pid == -1)
	{
        perror("pid");
        exit(EXIT_FAILURE);
    }
}

void execute_child_process(int *i, t_pipe *pipes, char **cmd_args, char ***envp, t_code *code, t_alloc **garbage, int *old_fd, t_command *command)
{
    if (*i > 0 && *old_fd != -1)
	{
        dup2(*old_fd, STDIN_FILENO);
        close(*old_fd);
    }
    if (command[*i].type == PIPE)
	{
        dup2(pipes->fd[1], STDOUT_FILENO);
        close(pipes->fd[1]);
    }
    close(pipes->fd[0]);
    if (execute_builtins(cmd_args, envp, code, garbage) == -1)
        execute_non_builtin(envp, code, cmd_args, garbage);
    exit(EXIT_SUCCESS);
}

void handle_parent_process(int *i, t_pipe *pipes, int *old_fd, pid_t pid, t_code *code, int *status, t_command *command)
{
    if (*i > 0 && *old_fd != -1)
        close(*old_fd);
    if (command[*i].type == PIPE)
	{
        *old_fd = pipes->fd[0];
        close(pipes->fd[1]);
    }
	else
	{
        close(pipes->fd[0]);
        close(pipes->fd[1]);
    }
    waitpid(pid, status, 0);
    if (WIFEXITED(*status))
        code->code_status = WEXITSTATUS(*status);
}
void ft_multipipes(t_command *command, t_alloc **garbage, char ***envp, char **cmd_args, int *i, t_code *code)
{
    t_pipe	pipes;
    pid_t	pid;
    int		status;
    int		old_fd;

	status = 0;
	old_fd = -1;
    while (command[*i].type != 0)
	{
        handle_command_args(command, &cmd_args, i, garbage);
        initialize_process(&pid, &pipes, command, i);
        if (pid == 0)
            execute_child_process(i, &pipes, cmd_args, envp, code, garbage, &old_fd, command);
		else
            handle_parent_process(i, &pipes, &old_fd, pid, code, &status, command);
        (*i)++;
    }
    if (old_fd != -1)
        close(old_fd);
}



/*
void	ft_multipipes(t_command *command, t_alloc **garbage, char ***envp, char **cmd_args, int *i, t_code *code)
{
	t_pipe	pipes;
	pid_t	pid;
	int		status;
	int		old_fd;

	status = 0;
	old_fd = -1;
	while (command[*i].type != 0)
	{
		if (command[*i].type == WORD)
			cmd_args = create_cmd_args(command, i, garbage);
		if (command[*i].type == PIPE || *i > 0)
			pipe(pipes.fd);
		pid = fork();
		if (pid == -1)
		{
			perror("pid");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (*i > 0 && old_fd != -1)
			{
				dup2(old_fd, STDIN_FILENO);
				close(old_fd);
			}
			if (command[*i].type == PIPE)
			{
				dup2(pipes.fd[1], STDOUT_FILENO);
				close(pipes.fd[1]);
			}
			close(pipes.fd[0]);
			if (execute_builtins(cmd_args, envp, code, garbage) == -1)
				execute_non_builtin(envp, code, cmd_args, garbage);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (*i > 0 && old_fd != -1)
			{
				close(old_fd);
				close(pipes.fd[1]);
			}
			if (command[*i].type == PIPE)
			{
				old_fd = pipes.fd[0];
				close(pipes.fd[1]);
			}
			else
				close(pipes.fd[1]);
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code->code_status = WEXITSTATUS(status);
		(*i)++;
	}
	if (old_fd != -1)
		close(old_fd);
}
*/