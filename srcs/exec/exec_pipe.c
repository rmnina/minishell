/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 00:47:33 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
void close_unused_fds(int *fd) {
    if (*fd != -1) {
        close(*fd);
        *fd = -1;
    }
}
*/
void update_exit_code(int status, t_code *code) {
    if (WIFEXITED(status)) {
        code->code_status = WEXITSTATUS(status);
    }
}
/*
void execute_pipeline(t_command *command, int num_commands, char ***envp, t_code *code, t_alloc *garbage) {
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];

    // Création des pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

      for (int i = 0; i < num_commands; i++) {
        if (pids[i] == 0) { // Processus enfant
            if (i > 0) { // Pour les commandes intermédiaires et la dernière commande
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
            }
            if (i < num_commands - 1) { // Pour les commandes intermédiaires et la première commande
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
            }
            // Fermer tous les autres fds de pipe
            for (int j = 0; j < num_commands - 1; j++) {
                if (j != i - 1) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            // Exécution de la commande
            char **cmd_args = create_cmd_args(&command[i], &i, garbage);
            if (execute_builtins(cmd_args, envp, code, garbage) == -1) {
                execute_non_builtin(envp, code, cmd_args, garbage);
            }
            exit(EXIT_SUCCESS);
        }
    }

    // Fermeture des fds de pipe dans le processus parent
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Attendre la fin de tous les processus enfants
    for (int i = 0; i < num_commands; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        update_exit_code(status, code);
    }
}
*/

void execute_pipeline(t_command *command, int num_commands, char ***envp, t_code *code, t_alloc *garbage) {
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];

    printf("Début de execute_pipeline avec %d commandes.\n", num_commands);
    // Création des pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        printf("Pipe créé entre les commandes %d et %d (fds: %d, %d)\n", i, i+1, pipes[i][0], pipes[i][1]);
    }

    // Lancement des processus pour chaque commande
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) 
        { // Processus enfant
            printf("Processus enfant %d démarré pour la commande %d.\n", getpid(), i);
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
   
            // Redirection de stdin et stdout
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][0]);
                printf("stdin redirigé pour la commande %d (fd: %d).\n", i, pipes[i - 1][0]);
            }
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][1]);
                printf("stdout redirigé pour la commande %d (fd: %d).\n", i, pipes[i][1]);
            }
            // Fermer tous les autres fds de pipe
            for (int j = 0; j < num_commands - 1; j++) {
                if (j != i - 1) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            // Exécution de la commande
            printf("command : %s\n", command[i].word);
            char **cmd_args = create_cmd_args(&command[i], &i, garbage);
            printf("Exécution de la commande %d : %s\n", i, cmd_args[0]);
            if (execute_builtins(cmd_args, envp, code, garbage) == -1) {
                execute_non_builtin(envp, code, cmd_args, garbage);
            }
            exit(EXIT_SUCCESS);
        }
        else
            printf("Processus parent %d a créé enfant %d pour commande %d.\n", getpid(), pids[i], i);
    }

    // Fermeture des fds de pipe dans le processus parent
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
        printf("Parent a fermé les fds de pipe %d et %d.\n", pipes[i][0], pipes[i][1]);
    }

    // Attendre la fin de tous les processus enfants
    for (int i = 0; i < num_commands; i++) 
	{
        int status;
        waitpid(pids[i], &status, 0);
        printf("Parent attend la fin du processus %d.\n", pids[i]);
        update_exit_code(status, code);
    }
    printf("Fin de execute_pipeline.\n");
}


/*
void parent_process_actions(t_command *command, int *i, t_pipe *pipes, int *old_fd) {
    
	printf("Début de parent_process_actions\n");
	if (*i > 0 && *old_fd != -1) {
        close(*old_fd);
		printf("Fermeture de l'ancien fd %d\n", *old_fd);
    }
    if (command[*i].type == PIPE) {
        *old_fd = pipes->fd[0];
        close(pipes->fd[1]);
		printf("Nouveau fd pour stdin est %d\n", *old_fd);
    } else {
        if (*old_fd != -1) {
            close(*old_fd);
            *old_fd = -1;
        }
    }
	printf("Fin de parent_process_actions\n");
}

void child_process_actions(t_command *command, int *i, char ***envp, char **cmd_args, t_code *code, t_alloc *garbage, t_pipe *pipes, int *old_fd) {
    printf("Début de child_process_actions pour la commande: %s\n", cmd_args[0]);
    
    printf("cmd_args : %s\n", cmd_args[0]);
	if (!is_builtin(cmd_args[0]) && *i == 0) 
	{
		close(pipes->fd[1]);
    }
	if (*i > 0 && *old_fd != -1) {
        dup2(*old_fd, STDIN_FILENO);
        close(*old_fd);
        printf("stdin redirigé de fd %d\n", *old_fd);
    }
    if (command[*i].type == PIPE) {
        dup2(pipes->fd[1], STDOUT_FILENO);
        close(pipes->fd[1]);
        printf("stdout redirigé vers fd %d\n", pipes->fd[1]);
    }
    close(pipes->fd[0]);

    if (execute_builtins(cmd_args, envp, code, garbage) == -1) {
        execute_non_builtin(envp, code, cmd_args, garbage);
    }

    printf("Fin de child_process_actions pour la commande: %s\n", cmd_args[0]);
}


void	ft_multipipes(t_command *command, t_alloc *garbage, char ***envp, char **cmd_args, int *i, t_code *code)
{
	t_pipe	pipes;
	pid_t	pid;
	int		status;
	int		old_fd;

	printf("Début de ft_multipipes\n");
	status = 0;
	old_fd = -1;
	while (command[*i].type != 0)
	{
		printf("Commande en cours: %s\n", command[*i].word);
		if (command[*i].type == WORD)
			cmd_args = create_cmd_args(command, i, garbage);
		if (command[*i].type == PIPE)
		{
			pipe(pipes.fd);
			printf("Pipe créé, fd[0] = %d, fd[1] = %d\n", pipes.fd[0], pipes.fd[1]);
        }
		pid = fork();
		if (pid == -1)
		{
			perror("pid");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			printf("Dans le processus enfant, PID = %d\n", getpid());
			child_process_actions(command, i, envp, cmd_args, code, garbage, &pipes, &old_fd);
			exit(EXIT_SUCCESS);
		}
		else // Processus parent
		{
			printf("Dans le processus parent, PID = %d\n", getpid());
			parent_process_actions(command, i, &pipes, &old_fd);
			waitpid(pid, &status, 0);
			update_exit_code(status, code);
		}
		(*i)++;
	}
	close_unused_fds(&old_fd);
	printf("Fin de ft_multipipes\n");
}
*/


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