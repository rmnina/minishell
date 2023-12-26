/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/25 19:26:07 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_line	*new_line(char *line, t_alloc *garbage)
{
	t_line	*node;

	node = malloc(sizeof(t_line));
	if (!node)
		return (NULL);
	node->line = ft_strdup(line, garbage);
	node->next = NULL;
	return (node);
}

void	free_line_nodes(t_line *head)
{
	t_line	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->line);
		free(temp);
	}
}

void	write_pipe(int fd, t_line *head)
{
	t_line	*current;

	current = head;
	while (current)
	{
		write(fd, current->line, ft_strlen(current->line));
		write(fd, "\n", 1);
		current = current->next;
	}

}


void	read_add(int fd, const char *delimiter, t_alloc *garbage)
{
	char	*line;
	t_line	*node;
	t_line	*head;
	t_line	*tail;
	
	head = NULL;
	tail = NULL;
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		node = new_line(line, garbage);
		free(line);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
	}
	write_pipe(fd, head);
	free_line_nodes(head);
	close(fd);
}

/*
int	heredoc(t_heredocNode *heredocList, t_pipe *pipes, char **argv, char **envp, t_alloc *garbage)
{
	pid_t	pid;
	int		status;
	int		code_status;
	t_heredocNode *current = heredocList;

	status = 0;
	code_status = 0;
	printf("Début de la fonction heredoc\n");
	while (current != NULL)
	{
		pipe(pipes->fd);
		printf("Pipe créé avec succès. pipes->fd[0]=%d, pipes->fd[1]=%d\n", pipes->fd[0], pipes->fd[1]);
		read_add(pipes->fd[1], current->delimiter, garbage);
		//close(pipes->fd[1]);
    	current = current->next;
	}
	printf("Lancement du processus parent\n");
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
        printf("rentre dans la fonction heredoc child");
		heredoc_child(pipes, argv, envp, garbage);
		exit (EXIT_SUCCESS);
	}
	else
	{
		close(pipes->fd[0]);
		close(pipes->fd[1]);
		printf("Attente de la fin du processus parent\n");
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code_status = WEXITSTATUS(status);
	}
	return (code_status);
}
*/


int heredoc(t_heredocNode *heredocList, char **argv, char **envp, t_alloc *garbage) {
    pid_t pid;
    int status;
    int common_pipe[2]; // Pipe commun pour le contenu des heredocs
	int code_status = 0;
	
    if (pipe(common_pipe) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    t_heredocNode *current = heredocList;
    printf("Début de la fonction heredoc\n");

    while (current != NULL) {
        int heredoc_pipe[2];
        if (pipe(heredoc_pipe) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Processus enfant
            close(heredoc_pipe[0]);
            read_add(heredoc_pipe[1], current->delimiter, garbage);
            exit(EXIT_SUCCESS);
        } else { // Processus parent
            close(heredoc_pipe[1]);
            // Lire depuis heredoc_pipe[0] et écrire dans common_pipe[1]
            char buffer[1024];
            ssize_t bytesRead;
            while ((bytesRead = read(heredoc_pipe[0], buffer, sizeof(buffer))) > 0) {
                write(common_pipe[1], buffer, bytesRead);
            }
            close(heredoc_pipe[0]);
            waitpid(pid, NULL, 0);
        }
        current = current->next;
    }
    close(common_pipe[1]); // Ferme l'écriture du pipe commun

    // Processus final pour exécuter la commande
    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        heredoc_child(common_pipe[0], argv, envp, garbage);
        exit(EXIT_SUCCESS);
    } else {
        close(common_pipe[0]); // Ferme la lecture du pipe commun
        waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code_status = WEXITSTATUS(status);
		printf("Processus final heredoc_child terminé.\n");
    }
    return (code_status);
}
