/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 14:42:23 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir(t_exec exec, char **argv, char **envp)
{
	int		fd;
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		if (exec.redirect_type == REDIRECT_OUTPUT)
		{
			fd = open(exec.file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
		}
		else if (exec.redirect_type == REDIRECT_INPUT)
		{
			fd = open(exec.file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
		}
		close(fd);
		path = find_command_path(exec.command);
		execve(path, argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
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