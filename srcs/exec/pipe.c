/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/16 01:04:56 by julietteand      ###   ########.fr       */
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
        ft_heredoc(main, i, garbage);
    else if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= RIGHT_CHEV)
    {
        (*main)->redir = 1;
    }
}

void	initialize_process(t_minishell **main, int *i)
{
	(void)i;
	//init_process_signal();
	
	pipe((*main)->fd);
	pipe((*main)->com);
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
	//printf("Initialisation du processus, PID : %d\n", (*main)->pid);
}

void	execute_child_process(t_minishell **main, int *i, t_alloc **garbage)
{

	close((*main)->fd[0]);
	if ((*main)->com[0] != -1)
		close((*main)->com[0]);
	if ((*main)->old_fd != -1)
	{
		dup2((*main)->old_fd, STDIN_FILENO);
		close((*main)->old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		dup2((*main)->fd[1], STDOUT_FILENO);
		close((*main)->fd[1]);
	}
	else
		close((*main)->fd[1]);
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		printf("Gestion du heredoc dans le processus enfant\n");
		printf("Ouverture du fichier heredoc : %s\n", (*main)->tmp_filename);
		int heredoc_fd = open((*main)->tmp_filename, O_RDONLY);
		printf("Après ouverture du fichier heredoc : %d\n", heredoc_fd);
        if (heredoc_fd == -1)
		{
            perror("Erreur lors de l'ouverture du fichier heredoc");
            exit(EXIT_FAILURE);
        }
        //Rediriger STDIN vers le fichier heredoc
       dup2(heredoc_fd, STDIN_FILENO);
       close(heredoc_fd);
	}
	if (next_is_pipe(main, i))
	{
		dup2((*main)->fd[0], STDIN_FILENO);
		close((*main)->fd[0]);
	}
	if ((*main)->redir)
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
		//printf("exécution de %s\n", (*main)->command[*i].word);
		if (execute_command(main, garbage) == -1)
			exit (EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(t_minishell **main, int *i, int *status)
{
	if ((*main)->com[1] != -1)
		close((*main)->com[1]);
	if ((*main)->fd[1] != -1)
		close((*main)->fd[1]);
	if ((*main)->old_fd != -1)
		close((*main)->old_fd);
	(*main)->old_fd = (*main)->fd[0];
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

	status = 0;
	handle_command_args(main, i, garbage);
	if (is_builtin((*main)->command[0].word) && (*main)->command[*i].type == 0)
	{
		execute_builtins(main, garbage);
		return (0);
	}
	initialize_process(main, i);
	if ((*main)->pid == 0)
		execute_child_process(main, i, garbage);
	else
		handle_parent_process(main, i, &status);
	(*i)++;
	if ((*main)->command[*i].type != 0)
		ft_pipex(main, i, garbage);
	if ((*main)->old_fd != -1)
		close((*main)->old_fd);
	if ((*main)->filefd != -1)
		close((*main)->filefd);
	return (1);
}
