/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/14 15:58:45 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
{
	if ((*main)->command[*i].type == WORD)
		(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= DB_LEFT_CHEV)
		return (0);
	return (1);
}

void	initialize_process(t_minishell **main, int *i)
{
	(void)i;
	//init_process_signal();
	if ((*main)->command[*i].type == PIPE || *i > 0)
	{
		//printf("Creating a pipe_fd in initialize process\n");
		pipe((*main)->pipe_fd);
	}
	//printf("Creating a pipe (*main)->com in initialize process\n");
	pipe((*main)->com);
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
}

/*
void	execute_child_process(t_minishell **main, int *i, int *old_fd, t_alloc **garbage)
{
	//if ((*main)->command[*i].type == DB_LEFT_CHEV) 
	//{
   //    printf("Heredoc suivi d'un pipe détecté\n");
    //    dup2((*main)->pipe_fd[0], STDIN_FILENO);
    //    close((*main)->pipe_fd[0]);
   //     close((*main)->pipe_fd[1]);
   //     
   // }
	if (*i > 0 && *old_fd != -1)
	{
		dup2(*old_fd, STDIN_FILENO);
		close(*old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		dup2((*main)->pipe_fd[1], STDOUT_FILENO);
	 	close((*main)->pipe_fd[1]);
	}
	close((*main)->pipe_fd[0]);
	if ((*main)->com[0] != -1)
		close((*main)->com[0]);
	if (!(*main)->redir)
		(*main)->redir = ft_redirect(main, i, garbage);
	if ((*main)->com[1] != -1)
	{
		write((*main)->com[1], i, sizeof(*i));
		close((*main)->com[1]);
	}
	if ((*main)->redir == -1)
		exit(EXIT_FAILURE);
	if (execute_builtins(main, garbage) == -1)
	{
		execute_command(main, garbage);
	}
	exit(EXIT_SUCCESS);
}
*/

void	execute_child_process(t_minishell **main, int *i, int *old_fd, t_alloc **garbage)
{
	if ((*main)->com[0] != -1)
		close((*main)->com[0]);
	if (*i > 0 && *old_fd != -1)
	{
		dup2(*old_fd, STDIN_FILENO);
		close(*old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		dup2((*main)->pipe_fd[1], STDOUT_FILENO);
		close((*main)->pipe_fd[1]);
	}
	if (!(*main)->redir)
		(*main)->redir = ft_redirect(main, i, garbage);
	if ((*main)->com[1] != -1)
	{
		write((*main)->com[1], i, sizeof(*i));
		close((*main)->com[1]);
	}
	if ((*main)->redir == -1)
		exit(EXIT_FAILURE);
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(t_minishell **main, int *i, int *old_fd, int *status)
{
	if ((*main)->com[1] != -1)
		close((*main)->com[1]);
	if (*i > 0 && *old_fd != -1)
		close(*old_fd);
	if ((*main)->command[*i].type == PIPE)
	{
		*old_fd = (*main)->pipe_fd[0];
		close((*main)->pipe_fd[1]);
	}
	else
	{
		close((*main)->pipe_fd[0]);
		close((*main)->pipe_fd[1]);
	}
	waitpid((*main)->pid, status, 0);
	if ((*main)->com[0] != -1)
	{
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
	}
	if (WIFEXITED(*status))
		(*main)->code_status = WEXITSTATUS(*status);
}

int 	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int		status;
	int		old_fd;

	status = 0;
	old_fd = -1;
	while ((*main)->command[*i].type != 0)
	{
		(*main)->redir = handle_command_args(main, i, garbage);
		initialize_process(main, i);
		if ((*main)->pid == 0)
			execute_child_process(main, i, &old_fd, garbage);
		else
			handle_parent_process(main, i, &old_fd, &status);
		(*i)++;
	}
	if (old_fd != -1)
		close(old_fd);
		
	return (1);
}