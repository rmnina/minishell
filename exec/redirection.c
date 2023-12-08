/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/08 17:57:55 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redirection_type(t_command *command, char *symbol, char *file)
{
	printf("set_redirection_type called with symbol: %s, file: %s\n", symbol, file);
	if (!ft_strcmp(symbol, "<<"))
		command->redirection_type = REDIRECT_APPEND_INPUT;
	else if (!ft_strcmp(symbol, ">"))
		command->redirection_type = REDIRECT_OUTPUT;
	else if (!ft_strcmp(symbol, ">>"))
		command->redirection_type = REDIRECT_APPEND_OUTPUT;
	else if (!ft_strcmp(symbol, "<"))
		command->redirection_type = REDIRECT_INPUT;
	command->redirection_file = file;
	printf("Redirection type set to: %d, Redirection file set to: %s\n", command->redirection_type, command->redirection_file);
}

void	redir_symbol(t_command *command, char **cmd_args)
{
	printf("redir_symbol called\n");
	int	i;

	i = 0;
	command->word = cmd_args[0];
	command->redirection_file = NULL;
	command->redirection_type = NO_REDIRECTION;

	while (cmd_args[i] != NULL)
	{
		printf("Checking cmd_arg[%d]: %s\n", i, cmd_args[i]);
		if (!ft_strcmp(cmd_args[i], "<<") || !ft_strcmp(cmd_args[i], ">") || \
			!ft_strcmp(cmd_args[i], ">>") || !ft_strcmp(cmd_args[i], "<"))
		{
			set_redirection_type(command, cmd_args[i], cmd_args[i + 1]);
			cmd_args[i] = NULL;
			if (cmd_args[i + 1] != NULL)
				cmd_args[i + 1] = NULL;
			break ;
		}
		i++;
	}
}

void	execute_redirection(t_command *command, char **argv, char **envp)
{
	int		fd;
	int		flags;
	char	*path;

	printf("execute_redirection started for command: %s\n", command->word);
	if (command->redirect_type == REDIRECT_OUTPUT || \
		command->redirect_type == REDIRECT_APPEND_OUTPUT)
	{
		if (command->redirect_type == REDIRECT_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (command->redirect_type == REDIRECT_APPEND_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(command->file, flags, 0644);
		printf("File descriptor for redirection: %d\n", fd);
		dup2(fd, STDOUT_FILENO);
		perror("Error during redirection setup");
	}
	else if (command->redirect_type == REDIRECT_INPUT)
	{
		fd = open(command->file, O_RDONLY);
		perror("Error during redirection setup");
		dup2(fd, STDIN_FILENO);
		perror("Error during redirection setup");

	}
	close(fd);
	path = find_command_path(command->word);
	printf("Executing command with redirection: %s\n", argv[0]);
	execve(path, argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	init_exec_struct(t_command *command)
{
	command->file = command->redirection_file;
	command->redirect_type = command->redirection_type;
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

// int handle_redirection(t_code *code, t_command *command, char **argv, char **envp)
// {
//     printf("handle_redirection called\n");
//     t_pipe pipes;
//     char **cmd_args1;
//     int hd_status = 0;
//     t_command *redir_command = command + 1;


//     cmd_args1 = create_cmd_args(command);
//     printf("cmd_args1 created: %s\n", cmd_args1[0]);
//     redir_symbol(redir_command, cmd_args1);
//     printf("Redirection setup: type=%d, file=%s\n", redir_command->redirection_type, redir_command->redirection_file);

//     if (redir_command->redirection_type == REDIRECT_APPEND_INPUT)
// 	{
//         hd_status = heredoc(redir_command->redirection_file, &pipes, cmd_args1, envp);
//         code->code_status = hd_status;
//         printf("handle_redirection finished for heredoc\n");
//     } 
// 	else if (redir_command->redirection_type != NO_REDIRECTION)
// 	{
//         execute_redirection(redir_command, argv, envp);
//         printf("handle_redirection finished for regular redirection\n");
//     }
// 	else 
// 	{
//         printf("No redirection required\n");
//     }

//     free_parsed_command_line(cmd_args1);
//     return hd_status;
// }

int handle_redirection(t_code *code, int *i, t_command *command, char **envp)
{
	printf("handle_redirection called\n");

	char		**cmd_args1;
	t_command	*redir_command;
	t_pipe		pipes;
	int			hd_status;

	hd_status = 0;
	cmd_args1 = NULL;
	redir_command = command + 1;
	if (command[*i].type >= LEFT_CHEV && command[*i].type <= DB_RIGHT_CHEV \
		&& command[*i + 1].type == WORD)
	{
		redir_command->redirection_type = command->type;
		redir_command->redirection_file = command[*i + 1].word;
		cmd_args1 = create_cmd_args(command, i, cmd_args1);
		if (redir_command->redirection_type == REDIRECT_APPEND_INPUT)
		{
			hd_status = heredoc(redir_command->redirection_file, &pipes, cmd_args1, envp);
			code->code_status = hd_status;
		}
		else if (redir_command->redirection_type != NO_REDIRECTION) 
			execute_redirection(redir_command, cmd_args1, envp);
		else
			printf("No redirection required\n");
	}
	free_parsed_command_line(cmd_args1);
	return (hd_status);
}

