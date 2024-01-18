/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/18 13:14:08 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	next_is_pipe(t_minishell **main, int *i)
// {
// 	int	j;

// 	if ((*main)->command[*i].type == PIPE)
// 		return (1);
// 	j = *i;
// 	while ((*main)->command[*i + j].type == WORD)
// 		j++;
// 	if ((*main)->command[*i + j].type == PIPE)
// 		return (1);
// 	return (0);
// }


// void	initialize_process(t_minishell **main, int *i)
// {
// 	(void)i;
// 	//init_process_signal();
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
// 	close((*main)->fd[0]);
// 	if ((*main)->com[0] != -1)
// 		close((*main)->com[0]);
// 	if ((*main)->old_fd != -1)
// 	{
// 		dup2((*main)->old_fd, STDIN_FILENO);
// 		close((*main)->old_fd);
// 	}
// 	if ((*main)->command[*i].type == PIPE)
// 	{
// 		printf("dup2 pour stdout : %d -> %d\n", (*main)->fd[1], STDOUT_FILENO);
// 		dup2((*main)->fd[1], STDOUT_FILENO);
// 		close((*main)->fd[1]);
// 	}
// 	else
// 	{
// 		close((*main)->fd[0]);
// 		//close((*main)->fd[1]);
// 	}
// 	if (next_is_pipe(main, i))
// 	{
// 		dup2((*main)->fd[0], STDIN_FILENO);
// 		close((*main)->fd[0]);
// 	}
// 	if ((*main)->redir)
// 		(*main)->redir = ft_redirect(main, i, garbage);
// 	if ((*main)->com[1] != -1)
// 	{
// 		write((*main)->com[1], i, sizeof(*i));
// 		close((*main)->com[1]);
// 	}
// 	if ((*main)->redir == -1)
// 	{
// 		exit(EXIT_FAILURE);
// 	}
// 	if (execute_builtins(main, garbage) == -1)
// 	{
// 		execute_command(main, garbage);
// 	}
// 	else
// 	{
// 		printf("Exécution de la commande : %s\n", (*main)->command[0].word);
// 	}
// 	exit(EXIT_SUCCESS);
// }


// void	handle_parent_process(t_minishell **main, int *i, int *status)
// {
// 	printf("Dans le processus parent, PID = %d\n", getpid());
// 	int	original_stdin;

// 	original_stdin = 0;
// 	(*main)->is_heredoc_used = true;
// 	if ((*main)->com[1] != -1)
// 		close((*main)->com[1]);
// 	if ((*main)->fd[1] != -1)
// 		close((*main)->fd[1]);
// 	if ((*main)->old_fd != -1)
// 		close((*main)->old_fd);
// 	(*main)->old_fd = (*main)->fd[0];
// 	waitpid((*main)->pid, status, 0);
// 	if ((*main)->is_heredoc_used)
// 	{
// 		original_stdin = open("/dev/tty", O_RDONLY);
// 		if (original_stdin < 0)
// 		{
// 			perror("Erreur lors de l'ouverture de /dev/tty");
// 			exit(EXIT_FAILURE);
// 		}
// 		dup2(original_stdin, STDIN_FILENO);
// 		close(original_stdin);

// 	}
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
// 	if ((*main)->filefd != -1)
// 		close((*main)->filefd);
// 	return (1);
// }


void	handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->redir = 0;
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		ft_heredoc(main, i, garbage);
		if ((*main)->cmd_args[*i] == NULL)
		{
			(*main)->cmd_args = NULL;
			(*main)->cmd_args = create_cmd_args(main, i, garbage);
		}
	}
	if ((*main)->command[*i].type >= LEFT_CHEV && \
	(*main)->command[*i].type <= RIGHT_CHEV)
	{	
		(*main)->redir = 1;
	}
}

void	initialize_process(t_minishell **main, int next_pipe)
{
	if (next_pipe)
		pipe((*main)->fd);
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
}

void	execute_child_process(t_minishell **main, int *i, t_alloc **garbage, int next_pipe)
{
	if ((*main)->redir == 1 || (*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->redir = ft_redirect(main, i, garbage);
	if ((*main)->redir == -1)
		exit(EXIT_FAILURE);
	if (*i > 0)
	{
		dup2((*main)->old_fd, STDIN_FILENO);
		close((*main)->old_fd);
	}
	if (next_pipe)
	{
		dup2((*main)->fd[1], STDOUT_FILENO);
		close((*main)->fd[1]);
	}
	close((*main)->fd[0]);
	if (execute_builtins(main, garbage) == -1)
	{
		execute_command(main, garbage);
	}
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(t_minishell **main, int *i, int *status, int next_pipe)
{
	int	original_stdin;

	original_stdin = 0;
	if (*i > 0)
		close((*main)->old_fd);
	if (next_pipe)
	{
		(*main)->old_fd = (*main)->fd[0];
		close((*main)->fd[1]);
	}
	else
	{
		close((*main)->fd[0]);
		close((*main)->fd[1]);
	}
	waitpid((*main)->pid, status, 0);
	if ((*main)->is_heredoc_used)
	{
		original_stdin = open("/dev/tty", O_RDONLY);
		if (original_stdin < 0)
			exit(EXIT_FAILURE);
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		(*main)->is_heredoc_used = false;
	}
	if (WIFEXITED(*status))
		(*main)->code_status = WEXITSTATUS(*status);
}

int	count_total_commands(t_command *command)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (command[0].word == NULL)
		return (0);
	while (command[i].word != NULL)
	{
		if (command[i].type == PIPE)
			count++;
		i++;
	}
	return (count + 1);
}



int ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int		status;
	int		j;
	int		cmd_index;
	int		next_pipe;
	int		num_commands;
	int 	is_first_command_cat;
	pid_t	*pids;

	(*main)->is_heredoc_used = true;
	status = 0;
	is_first_command_cat = 0;
	next_pipe = 0;
	cmd_index = 0;
	j = 0;
	num_commands = count_total_commands((*main)->command);
	pids = malloc(sizeof(pid_t) * num_commands);
	if (!pids)
		return (-1);
	if (ft_strcmp((*main)->command[0].word, "cat") == 0 && \
	(*main)->command[1].type == PIPE)
	{
			is_first_command_cat = 1;
			(*i)++;
	}
	while ((*main)->command[*i].word != NULL)
	{
		handle_command_args(main, i, garbage);
		if (is_builtin((*main)->command[0].word) && \
		(*main)->command[*i].type == 0)
		{
			execute_builtins(main, garbage);
			return (0);
		}
		next_pipe = ((*main)->command[*i + 1].word != NULL && \
		(*main)->command[*i].type == PIPE);
		initialize_process(main, next_pipe);
		if ((*main)->pid == 0)
			execute_child_process(main, i, garbage, next_pipe);
		else
		{
			pids[cmd_index++] = (*main)->pid;
			handle_parent_process(main, i, &status, next_pipe);
		}
		while ((*main)->command[*i].word != NULL && \
		(*main)->command[*i].type != PIPE)
			(*i)++;
		if ((*main)->command[*i].word != NULL && \
		(*main)->command[*i].type == PIPE)
			(*i)++;
		if ((*main)->filefd != -1)
			close((*main)->filefd);
		j++;
	}
	j = 0;
	while (j < cmd_index)
	{
		waitpid(pids[j], &status, 0);
		j++;
	}
	free(pids);
	if (is_first_command_cat)
	{
		*i = 0;
		handle_command_args(main, i, garbage);
		initialize_process(main, 0);
		if ((*main)->pid == 0)
			execute_child_process(main, i, garbage, 0);
		else
			waitpid((*main)->pid, &status, 0);
	}
	return (1);
}

