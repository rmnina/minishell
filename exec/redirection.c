/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/23 18:35:08 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redir(t_exec *exec, char **argv, char **envp)
{
	int		fd;
	pid_t	pid;
	char	*path;
	int		flags;

	pid = fork();
	if (pid == 0)
	{
		if (exec->redirect_type == REDIRECT_OUTPUT \
			|| exec->redirect_type == REDIRECT_APPEND_OUTPUT)
		{
			if (exec->redirect_type == REDIRECT_OUTPUT)
				flags = O_WRONLY | O_CREAT | O_TRUNC;
			else
				flags = O_WRONLY | O_CREAT | O_APPEND;
			fd = open(exec->file, flags, 0644);
			dup2(fd, STDOUT_FILENO);
		}
		else if (exec->redirect_type == REDIRECT_APPEND_INPUT \
			|| exec->redirect_type == REDIRECT_INPUT)
		{
			fd = open(exec->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
		}
		close(fd);
		path = find_command_path(exec->command);
		execve(path, argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}

void	init_exec_struct(t_exec *exec)
{
	exec->file = exec->redirection_file;
	exec->redirect_type = exec->redirection_type;
}

void	redir_symbol(t_exec *exec, char **cmd_args)
{
	int		i;

	exec->command = cmd_args[0];
	exec->redirection_file = NULL;
	exec->redirection_type = NO_REDIRECTION;
	i = 0;
	while (cmd_args[i] != NULL)
	{
		if (!strcmp(cmd_args[i], "<<"))
		{
			exec->redirection_type = REDIRECT_APPEND_INPUT;
			if (cmd_args[i + 1] != NULL)
				exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		if (strcmp(cmd_args[i], ">") == 0)
		{
			exec->redirection_type = REDIRECT_OUTPUT;
			exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		else if (strcmp(cmd_args[i], ">>") == 0)
		{
			exec->redirection_type = REDIRECT_APPEND_OUTPUT;
			exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		else if (strcmp(cmd_args[i], "<") == 0)
		{
			exec->redirection_type = REDIRECT_INPUT;
			exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		i++;
	}
}

int	handle_redirection(t_exec *exec, char *input, char **argv, char **envp)
{
	t_pipe	pipes;
	char	**cmd_args;

	cmd_args = parse_command_line(input);
	redir_symbol(exec, cmd_args);
	if (exec->redirect_type == REDIRECT_APPEND_INPUT)
	{
		here_doc(exec->redirection_file, &pipes, &exec->command, envp);
		free_parsed_command_line(cmd_args);
		return (1);
	}
	if (exec->redirection_type != NO_REDIRECTION)
	{
		init_exec_struct(exec);
		redir(exec, cmd_args, envp);
		free_parsed_command_line(cmd_args);
		return (1);
	}
	free_parsed_command_line(cmd_args);
	return (0);
}


// int	main(int argc, char **argv, char **envp) 
// {
// 	(void)argc;
// 	(void)argv;
// 	t_exec exec_out = {
// 		.command = "ls",
//         .file = "output.txt",
//         .redirect_type = REDIRECT_OUTPUT
//     };
//     char *argv_ls[] = {"ls", NULL};
//     redir(exec_out, argv_ls, envp);

//     t_exec exec_in = {
//         .command = "cat",
//         .file = "output.txt",
//         .redirect_type = REDIRECT_INPUT
//     };
//     char *argv_cat[] = {"cat", NULL};
//     redir(exec_in, argv_cat, envp);

//     return 0;
// }