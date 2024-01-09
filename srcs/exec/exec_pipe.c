/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 17:16:42 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_multipipes(t_command *command, t_alloc **garbage, char ***envp, char **cmd_args, int *i, t_code *code)
{
	t_pipe	pipes;
	pid_t	pid;
	int		status;
	int		old_fd;

	status = 0;
	old_fd = -1;
	while (command[*i].type && command[*i].type != 0)
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

// void	execute_pipeline(t_command *command, int num_commands, char ***envp, t_code *code, t_alloc *garbage)
// {
// 	int		pipes[num_commands - 1][2];
// 	pid_t	pids[num_commands];

// 	//printf("Début de execute_pipeline avec %d commandes.\n", num_commands);
// 	// Création des pipes
// 	for (int i = 0; i < num_commands - 1; i++) {
// 		if (pipe(pipes[i]) == -1) {
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 		//printf("Pipe créé entre les commandes %d et %d (fds: %d, %d)\n", i, i+1, pipes[i][0], pipes[i][1]);
// 	}

// 	// Lancement des processus pour chaque commande
// 	for (int i = 0; i < num_commands; i++)
// 	{
// 		printf("execute_pipeline: Création du processus pour la commande %s\n", command[i].word);
// 		pids[i] = fork();
// 		if (pids[i] == -1) {
// 			perror("fork");
// 			exit(EXIT_FAILURE);
// 		}

// 		if (pids[i] == 0) 
// 		{ // Processus enfant
// 			//printf("Processus enfant %d démarré pour la commande %d.\n", getpid(), i);
// 			signal(SIGINT, SIG_DFL);
// 			signal(SIGQUIT, SIG_DFL);
   
// 			// Redirection de stdin et stdout
// 			if (i > 0) {
// 				dup2(pipes[i - 1][0], STDIN_FILENO);
// 				close(pipes[i - 1][0]);
// 				//printf("stdin redirigé pour la commande %d (fd: %d).\n", i, pipes[i - 1][0]);
// 			}
// 			if (i < num_commands - 1) {
// 				dup2(pipes[i][1], STDOUT_FILENO);
// 				close(pipes[i][1]);
// 				//printf("stdout redirigé pour la commande %d (fd: %d).\n", i, pipes[i][1]);
// 			}
// 			// Fermer tous les autres fds de pipe
// 			for (int j = 0; j < num_commands - 1; j++) {
// 				if (j != i - 1) close(pipes[j][0]);
// 				if (j != i) close(pipes[j][1]);
// 			}

// 			// Exécution de la commande
// 			//printf("command : %s\n", command[i].word);
// 			char **cmd_args = create_cmd_args(&command[i], &i, garbage);
// 			//printf("Exécution de la commande %d : %s\n", i, cmd_args[0]);
// 			if (execute_builtins(cmd_args, envp, code, garbage) == -1) {
// 				execute_non_builtin(envp, code, cmd_args, garbage);
// 			}
// 			exit(EXIT_SUCCESS);
// 		}
// 		//else
// 			//printf("Processus parent %d a créé enfant %d pour commande %d.\n", getpid(), pids[i], i);
// 	}

// 	// Fermeture des fds de pipe dans le processus parent
// 	for (int i = 0; i < num_commands - 1; i++) {
// 		close(pipes[i][0]);
// 		close(pipes[i][1]);
// 		//printf("Parent a fermé les fds de pipe %d et %d.\n", pipes[i][0], pipes[i][1]);
// 	}

// 	// Attendre la fin de tous les processus enfants
// 	for (int i = 0; i < num_commands; i++) 
// 	{
// 		int status;
// 		waitpid(pids[i], &status, 0);
// 		update_exit_code(status, code);
// 	}
// 	//printf("Fin de execute_pipeline.\n");
// }





/*
void	ft_multipipes(t_command *command, t_alloc *garbage, char ***envp, char **cmd_args, int *i, t_code *code)
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