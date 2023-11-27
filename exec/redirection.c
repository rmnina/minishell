/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/27 18:34:49 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	redir(t_exec *exec, char **argv, char **envp)
{
	int		fd;
	pid_t	pid;
	char	*path;
	int		flags;
	
	printf("redir: Redirecting for command '%s', Type %d, File '%s'\n", exec->command, exec->redirect_type, exec->file);
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
		else if  (exec->redirect_type == REDIRECT_INPUT) // (exec->redirect_type == REDIRECT_APPEND_INPUT 
		{
			fd = open(exec->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		//close(fd);
		path = find_command_path(exec->command);
		printf("redir: Executing command '%s'\n", exec->command);
for (int i = 0; argv[i] != NULL; i++) {
    printf("argv[%d] = '%s'\n", i, argv[i]);
}

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
		if (!ft_strcmp(cmd_args[i], "<<"))
		{
			exec->redirection_type = REDIRECT_APPEND_INPUT;
			if (cmd_args[i + 1] != NULL) 
			{
				exec->redirection_file = cmd_args[i + 1];
				cmd_args[i] = NULL;
				cmd_args[i + 1] = NULL;
				break ;
			}
		}
		else if (ft_strcmp(cmd_args[i], ">") == 0)
		{
			exec->redirection_type = REDIRECT_OUTPUT;
			exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		else if (ft_strcmp(cmd_args[i], ">>") == 0)
		{
			exec->redirection_type = REDIRECT_APPEND_OUTPUT;
			exec->redirection_file = cmd_args[i + 1];
			cmd_args[i] = NULL;
			break ;
		}
		else if (ft_strcmp(cmd_args[i], "<") == 0)
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
	char	**cmd_args1;

	printf("Handling redirection for input: %s\n", input);
	cmd_args1 = parse_command_line(input);

	for (int i = 0; cmd_args1[i] != NULL; i++)
	{
    	printf("cmd_args1[%d] = %s\n", i, cmd_args1[i]);
	}
	redir_symbol(exec, cmd_args1);
	if (exec->redirection_type == REDIRECT_APPEND_INPUT)
	{
		printf("handle_redirection: Command '%s', Type %d, File '%s'\n", exec->command, exec->redirect_type, exec->redirection_file);
		here_doc(exec->redirection_file, &pipes, cmd_args1, envp);
		printf("handle_redirection: Redirection completed for '%s'\n", exec->command);
		free_parsed_command_line(cmd_args1);
		printf("Redirection handled\n");
		return (1);
	}
	else if (exec->redirection_type != NO_REDIRECTION)
	{
		init_exec_struct(exec);
		printf("handle_redirection: Command '%s', Type %d, File '%s'\n", exec->command, exec->redirect_type, exec->redirection_file);
		redir(exec, cmd_args1, envp);
		printf("handle_redirection: Redirection completed for '%s'\n", exec->command);
		free_parsed_command_line(cmd_args1);
		return (1);
	}
	free_parsed_command_line(cmd_args1);
	printf("Redirection handled\n");
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