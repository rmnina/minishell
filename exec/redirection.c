/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/25 19:17:41 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redir_output(char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR \
	| S_IRGRP | S_IWGRP | S_IWUSR);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (dup == -1)
		return (-1);
	return (dup);
}

int	redir_append(char *filename)
{
	int	fd;
	int	dup;

	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd == -1)
		return (-1);
	dup = dup2(fd, STDOUT_FILENO);
	if (dup == -1)
		return (-1);
	return (fd);
}

int	redir_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (-1);
	return (fd *= -1);
}

t_heredocNode *buildHeredocList(t_command *command, int *i)
{
    t_heredocNode *head = NULL, *current = NULL;

    while (command[*i].type == DB_LEFT_CHEV)
    {
        t_heredocNode *newNode = malloc(sizeof(t_heredocNode));
        if (!newNode) {
            perror("Failed to allocate memory for heredoc node");
            break;
        }
        newNode->delimiter = command[*i + 1].word;
        newNode->next = NULL;

        if (!head) {
            head = newNode;
        } else {
            current->next = newNode;
        }
        current = newNode;
        *i += 2;
    }

    return head;
}



int	init_redirection(t_command *command, int *i, char **cmd_args, char **envp, t_code *code)
{
	char	*filename;
	int		fd;
	pid_t	pid;
	int		status;
	t_alloc	*son_garb;
	//t_pipe	pipes;

	son_garb = NULL;
	fd = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		t_heredocNode *heredocList = buildHeredocList(command, i);
		if (heredocList)
        {
			heredoc(heredocList, cmd_args, envp, son_garb);
			exit(EXIT_SUCCESS);
        }
		//if (command[*i].type == DB_LEFT_CHEV)
		//{
			//heredoc(command[*i + 1].word, &pipes, cmd_args, envp, son_garb);
			
			//exit(EXIT_SUCCESS);
		//}
		if (command[*i].type == DB_RIGHT_CHEV || \
		command[*i].type == RIGHT_CHEV)
		{
			filename = ft_strdup(command[*i + 1].word, son_garb);
			if (!filename)
				return (-1);
			if (command[*i].type == RIGHT_CHEV)
				fd = redir_output(filename);
			else if (command[*i].type == DB_RIGHT_CHEV)
				fd = redir_append(filename);
		}
		else if (command[*i].type == LEFT_CHEV)
		{
			filename = command[*i - 1].word;
			fd = redir_input(filename);
		}
		if (execute_builtins(cmd_args, envp, code, son_garb) == -1)
			execute_non_builtin(envp, code, cmd_args, son_garb);
		free_garbage(&son_garb, 0);
		exit(EXIT_SUCCESS);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
		if (fd < -1)
			dup2(STDIN_FILENO, fd * -1);
		else if (fd > 0)
			dup2(STDOUT_FILENO, fd);
		}
	}
	*i += 2;
	return (1); 
}