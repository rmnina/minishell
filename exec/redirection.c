/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/07 17:03:16 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redirection_type(t_command *command, char *symbol, char *file)
{
	if (!ft_strcmp(symbol, "<<"))
		command->redirection_type = REDIRECT_APPEND_INPUT;
	else if (!ft_strcmp(symbol, ">"))
		command->redirection_type = REDIRECT_OUTPUT;
	else if (!ft_strcmp(symbol, ">>"))
		command->redirection_type = REDIRECT_APPEND_OUTPUT;
	else if (!ft_strcmp(symbol, "<"))
		command->redirection_type = REDIRECT_INPUT;
	command->redirection_file = file;
}

void	redir_symbol(t_command *command, char **cmd_args)
{
	int	i;

	i = 0;
	command->word = cmd_args[0];
	command->redirection_file = NULL;
	command->redirection_type = NO_REDIRECTION;

	while (cmd_args[i] != NULL)
	{
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

	if (command->redirect_type == REDIRECT_OUTPUT || \
		command->redirect_type == REDIRECT_APPEND_OUTPUT)
	{
		if (command->redirect_type == REDIRECT_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (command->redirect_type == REDIRECT_APPEND_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(command->file, flags, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	else if (command->redirect_type == REDIRECT_INPUT)
	{
		fd = open(command->file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
	}
	close(fd);
	path = find_command_path(command->word);
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

int handle_redirection(t_code *code, t_command *command, int command_start_index, char **argv, char **envp)
{
	printf("handle_redirection called\n");

	char		**cmd_args1;
	t_command	*redir_command;
	t_pipe		pipes;
	int			hd_status = 0;

	redir_command = command + 1;
	(void)argv;
	if (command[0].type >= LEFT_CHEV && command[0].type <= DB_RIGHT_CHEV && command[1].type == WORD)
	{
		redir_command->redirection_type = command->type;
		redir_command->redirection_file = command[1].word;
		cmd_args1 = create_cmd_args(command - command_start_index);
		if (redir_command->redirection_type == REDIRECT_APPEND_INPUT)
		{
			hd_status = heredoc(redir_command->redirection_file, &pipes, cmd_args1, envp);
			code->code_status = hd_status;
		}
		else if (redir_command->redirection_type != NO_REDIRECTION) 
		{
			execute_redirection(redir_command, cmd_args1, envp);
		}
	}
	free_parsed_command_line(cmd_args1);
	return (hd_status);
}

