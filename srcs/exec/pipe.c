/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/16 18:58:18 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	next_is_pipe(t_minishell **main, int *i)
{
	int	j;

	if ((*main)->command[*i].type == PIPE)
		return (1);
	j = *i;
	while ((*main)->command[*i + j].type == WORD)
		j++;
	if ((*main)->command[*i + j].type == PIPE)
		return (1);
	return (0);
}

void	handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->redir = 0;
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		ft_heredoc(main, i, garbage);
	}
	else if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= RIGHT_CHEV)
		(*main)->redir = 1;
}

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


void initialize_process(t_minishell **main, int *i)
{
	if (*i > 0) { // S'il ne s'agit pas de la première commande dans le pipeline
		pipe((*main)->fd);
	}
	(*main)->pid = fork();
	if ((*main)->pid == -1) {
		perror("pid");
		exit(EXIT_FAILURE);
	}
}

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

void execute_child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->fd[0]);
	if (*i > 0)
	{
		dup2((*main)->old_fd, STDIN_FILENO);
		close((*main)->old_fd);
	}
	// close((*main)->fd[0]);
	if ((*main)->command[*i].type == PIPE)
	{
		dup2((*main)->fd[0], STDIN_FILENO);
		close((*main)->fd[0]);
	}
	if (execute_builtins(main, garbage) == -1)
	{
		execute_command(main, garbage);
	}
	else
	{
		printf("Exécution de la commande : %s\n", (*main)->command[0].word);
	}
}


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

void handle_parent_process(t_minishell **main, int *i, int *status)
{
	if (*i > 0)
	{
		close((*main)->old_fd);
	}
	(*main)->old_fd = (*main)->fd[0];
	close((*main)->fd[1]);
	waitpid((*main)->pid, status, 0);
	if (WIFEXITED(*status))
		(*main)->code_status = WEXITSTATUS(*status);
}


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

int count_commands(t_minishell **main, int *i)
{
	int count = 0;
	while ((*main)->command[*i].word != NULL)
	{
		if ((*main)->command[*i].type != PIPE)
		{
			printf("nombre de commandes: %d, commande: %s\n", count, (*main)->command[*i].word);
			count++;
		}
		(*i)++;
	}
	return count;
}



int ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int status;
	int cmd_count = count_commands(main, i);
	printf("Nombre total de commandes: %d\n", cmd_count);
	pid_t *pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return -1;
	int cmd_index = 0;

	handle_command_args(main, i, garbage);
	for (int j = 0; (*main)->command[*i + j].word != NULL; j++)
{
	if ((*main)->command[*i + j].type == PIPE)
	{
		j++; // Passer le PIPE
		if ((*main)->command[*i + j].word == NULL) break; // Fin de la commande
	}

	printf("Traitement de la commande %d: %s\n", j, (*main)->command[*i + j].word);
	initialize_process(main, i);
	if ((*main)->pid == 0)
	{
		execute_child_process(main, i + j, garbage);
	}
	else
	{
		pids[cmd_index++] = (*main)->pid;
		handle_parent_process(main, i, &status);
	}
}
	free(pids);
	return 1;
}
