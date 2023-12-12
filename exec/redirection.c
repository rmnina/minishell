/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/12 16:26:45 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redirection_type(t_command *command, char *symbol, char *file)
{
	printf("Setting redirection type. Symbol: '%s', File: '%s'\n", symbol, file);
	if (!ft_strcmp(symbol, "<<"))
		command->type = DB_LEFT_CHEV;
	else if (!ft_strcmp(symbol, ">"))
		command->type = RIGHT_CHEV;
	else if (!ft_strcmp(symbol, ">>"))
		command->type = DB_RIGHT_CHEV;
	else if (!ft_strcmp(symbol, "<"))
		command->type = LEFT_CHEV;
	command->redirection_file = file;
}

// void	redir_symbol(t_command *command, char **cmd_args)
// {
// 	printf("Setting redirection symbol.\n");
// 	int	i;

// 	i = 0;
// 	command->word = cmd_args[0];
// 	command->redirection_file = NULL;
// 	command->redirection_type = NO_REDIRECTION;

// 	while (cmd_args[i] != NULL)
// 	{
// 		if (!ft_strcmp(cmd_args[i], "<<") || !ft_strcmp(cmd_args[i], ">") || 
// 			!ft_strcmp(cmd_args[i], ">>") || !ft_strcmp(cmd_args[i], "<"))
// 		{
// 			set_redirection_type(command, cmd_args[i], cmd_args[i + 1]);
// 			cmd_args[i] = NULL;
// 			if (cmd_args[i + 1] != NULL)
// 				cmd_args[i + 1] = NULL;
// 			break ;
// 		}
// 		i++;
// 	}
// }
void redir_symbol(t_command *command)
{
	printf("redir symbol started\n");
	int i = 0;
	char	**cmd_args = create_cmd_args(command);

	while (cmd_args[i] != NULL)
	{
		if (!ft_strcmp(cmd_args[i], "<<") || !ft_strcmp(cmd_args[i], ">") || \
			!ft_strcmp(cmd_args[i], ">>") || !ft_strcmp(cmd_args[i], "<"))
		{
			set_redirection_type(command, cmd_args[i], cmd_args[i + 1]);
			break ;
		}
		i++;
	}
	free_parsed_command_line(cmd_args);
	printf("redir symbol finished\n");
}


// void	execute_redirection(t_command *command, char **argv, char **envp)
// {
// 	int		fd;
// 	int		flags;
// 	char	*path;

// 	if (command->redirect_type == REDIRECT_OUTPUT || 
// 		command->redirect_type == REDIRECT_APPEND_OUTPUT)
// 	{
// 		if (command->redirect_type == REDIRECT_OUTPUT)
// 			flags = O_WRONLY | O_CREAT | O_TRUNC;
// 		else if (command->redirect_type == REDIRECT_APPEND_OUTPUT)
// 			flags = O_WRONLY | O_CREAT | O_APPEND;
// 		fd = open(command->file, flags, 0644);
// 		dup2(fd, STDOUT_FILENO);
// 	}
// 	else if (command->redirect_type == REDIRECT_INPUT)
// 	{
// 		fd = open(command->file, O_RDONLY);
// 		dup2(fd, STDIN_FILENO);
// 	}
// 	close(fd);
// 	path = find_command_path(command->word);
// 	execve(path, argv, envp);
// 	perror("execve");
// 	exit(EXIT_FAILURE);
// }

void execute_redirection(t_command *command, char **cmd_args, char **envp) 
{
	printf("execute_redirection started\n");
	int		fd;
	int		flags;
	char	*path;
	pid_t	pid;
	
	fd = 0;
	flags = 0;
	pid = fork();
	if (pid == 0)
	{
		printf("execute_redirection: dans le processus enfant\n");
		if (command->type == RIGHT_CHEV || 
			command->type == DB_RIGHT_CHEV
			|| command->type == LEFT_CHEV)
		{
				if (command->type == RIGHT_CHEV)
					flags = O_WRONLY | O_CREAT | O_TRUNC;
				else if (command->type == DB_RIGHT_CHEV)
					flags = O_WRONLY | O_CREAT | O_APPEND;
				fd = open(command->file, flags, 0644);
				dup2(fd, STDOUT_FILENO);
				printf("Output redirection set up successfully\n");
		}
		else if (command->type == LEFT_CHEV)
		{
			fd = open(command->redirection_file, O_RDONLY);
			printf("Opening file for input redirection: %s, fd: %d\n", command->redirection_file, fd);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			printf("Input redirection set up successfully\n");
		}
		close(fd);
		printf("contenu de cmd args[0] avant path de printf: %s\n", command->word);
		path = find_command_path(command->word);
		printf("Path found for command '%s': %s\n", command->word, path ? path : "NULL");
		if (path == NULL) 
		{
            printf("execute_redirection: chemin de commande introuvable\n");
            exit(EXIT_FAILURE);
        }
		printf("contenu de path avant execve de printf: %s\n", path);
		execve(path, cmd_args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	init_exec_struct(t_command *command)
{
	printf("Initializing execution structure. File: '%s', Redirect Type: %d\n", command->redirection_file, command->redirection_type);
	command->file = command->redirection_file;
	command->redirect_type = command->type;
}

// int	handle_redirection(t_code *code, t_command *command, char **argv, char **envp)
// {
// 	t_pipe		pipes;
// 	char		**cmd_args1;
// 	int			hd_status;

// 	(void)argv;
// 	hd_status = 0;
// 	printf("handle_redirection called\n");
// 	cmd_args1 = create_cmd_args(command);
// 	printf("cmd_args1 created: %s\n", cmd_args1[0]);
// 	redir_symbol(command, cmd_args1);
// 	printf("Redirection setup: type = %d, file = %s\n", command->redirection_type, command->redirection_file);
// 	if (command->redirection_type == REDIRECT_APPEND_INPUT)
// 	{
// 		hd_status = heredoc(command->redirection_file, &pipes, cmd_args1, envp);
// 		code->code_status = hd_status;
// 		free_parsed_command_line(cmd_args1);
// 		printf("handle_redirection finished for heredoc\n"); 
// 		return (1);
// 	}
// 	else if (command->redirection_type != NO_REDIRECTION)
// 	{
// 		init_exec_struct(command);
// 		pid_redir(command, cmd_args1, envp, code);
// 		free_parsed_command_line(cmd_args1);
// 		printf("handle_redirection finished for regular redirection\n");
// 		return (1);
// 	}
// 	free_parsed_command_line(cmd_args1);
// 	printf("handle_redirection finished with no redirection\n");
// 	return (0);

// }

// int handle_redirection(t_code *code, t_command *command, char **envp)
// {
// 	char **cmd_args;


// 	printf("handle_redirection called\n");
// 	t_pipe	pipes;
// 	int		hd_status;

// 	hd_status = 0;
// 	redir_symbol(command);
// 	cmd_args = create_cmd_args(command);
// 	printf("cmd_args: %s\n", cmd_args[0]);
// 	if (command->type >= LEFT_CHEV && command->type <= DB_RIGHT_CHEV)
// 	{
// 		execute_redirection(command, cmd_args, envp);
// 	}
// 	if (command->type == WORD)
// 	{
// 		char **cmd_args = create_cmd_args(command);
// 		printf("command args are created for WORD type:\n");
// 		for (int i = 0; cmd_args[i] != NULL; i++)
// 		{
// 			printf("  Arg %d: %s\n", i, cmd_args[i]);
// 		}
// 		printf("Executing command: %s\n", cmd_args[0]);
// 		execute_command(cmd_args, envp);
// 		free_parsed_command_line (cmd_args);
// 	}
// 	else if (command->type == DB_LEFT_CHEV)
// 	{
// 		cmd_args = create_cmd_args(command);
// 		printf("cmd_args created for heredoc: %s\n", cmd_args[0]);
// 		hd_status = heredoc(command->redirection_file, &pipes, cmd_args, envp);
// 		code->code_status = hd_status;
// 		printf("handle_redirection finished for heredoc\n");
// 		free_parsed_command_line(cmd_args);
// 	}
// 	printf("Redirection handling complete. Status: %d\n", hd_status);
// 	return (hd_status);
// }

int handle_redirection(t_code *code, t_command *command, char **envp)
{
    char **cmd_args;

    if (!command) {
        fprintf(stderr, "Command structure is NULL in handle_redirection.\n");
        return -1;
    }
    printf("handle_redirection called\n");
    t_pipe pipes;
    int hd_status = 0;

    redir_symbol(command);
    cmd_args = create_cmd_args(command); 
    printf("cmd_args: %s\n", cmd_args[0]);

    if (command->type != NO_REDIRECTION) {
        execute_redirection(command, cmd_args, envp);
    } else {
        printf("Executing command: %s\n", cmd_args[0]);
        execute_command(cmd_args, envp);
    }

    if (command->type == DB_LEFT_CHEV) {
        hd_status = heredoc(command->redirection_file, &pipes, cmd_args, envp);
        code->code_status = hd_status;
        printf("handle_redirection finished for heredoc\n");
    }

    free_parsed_command_line(cmd_args);
    printf("Redirection handling complete. Status: %d\n", hd_status);
    return hd_status;
}


// int handle_redirection(t_code *code, int *i, t_command *command, char **envp)
// {
// 	printf("handle_redirection called\n");

// 	char		**cmd_args1;
// 	t_command	*redir_command;
// 	t_pipe		pipes;
// 	int			hd_status = 0;

// 	redir_command = command + 1;
// 	(void)argv;
// 	if (command[0].type >= LEFT_CHEV && command[0].type <= DB_RIGHT_CHEV && command[1].type == WORD)
// 	{
// 		redir_command->redirection_type = command->type;
// 		redir_command->redirection_file = command[1].word;
// 		cmd_args1 = create_cmd_args(command - command_start_index);
// 		if (redir_command->redirection_type == REDIRECT_APPEND_INPUT)
// 		{
// 			hd_status = heredoc(redir_command->redirection_file, &pipes, cmd_args1, envp);
// 			code->code_status = hd_status;
// 		}
// 		else if (redir_command->redirection_type != NO_REDIRECTION) 
// 			execute_redirection(redir_command, cmd_args1, envp);
// 		}
// 	}
// 	free_parsed_command_line(cmd_args1);
// 	return (hd_status);
// }

