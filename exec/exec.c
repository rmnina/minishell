/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/21 19:01:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtins(char **cmd_args, char **envp, t_code *code, t_alloc *garbage)
{
	if (cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp(cmd_args[0], "cd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "echo") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("echo"))
		return (ft_echo(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "env") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("env"))
		return (ft_env(envp, code));
	if (ft_strcmp(cmd_args[0], "exit") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("exit"))
		return (ft_exit(cmd_args, code, garbage));
	if (ft_strcmp(cmd_args[0], "export") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("export"))
		return (ft_export(envp, code));
	if (ft_strcmp(cmd_args[0], "pwd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("pwd"))
		return (ft_pwd(NULL, NULL, code));
	if (ft_strcmp(cmd_args[0], "unset") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("unset"))
		return (ft_unset(&envp, cmd_args + 1, code));
	return (-1);
}

int	execute_non_builtin(char **envp, t_code *code, char **cmd_args, t_alloc *garbage)
{
	//printf("execute_non_builtin: Entrée\n");
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
	{
		perror ("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		//printf("execute_non_builtin: Dans le processus enfant\n");
		execute_command(cmd_args, envp, garbage);
		//printf("execute_non_builtin: Processus enfant terminé\n");
        exit(EXIT_SUCCESS);
	}
	else
	{
		//printf("execute_non_builtin: Dans le processus parent, en attente de l'enfant\n");
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			code->code_status = WEXITSTATUS(status);
			//printf("execute_non_builtin: Processus enfant s'est terminé avec le statut: %d\n", code->code_status);
		}
	}
	//printf("execute_non_builtin: Sortie\n");
	return (-1);
}

void	heredoc_child(t_pipe *pipes, char **argv, char **envp, t_alloc *garbage)
{
	char	*path;
	char	*new_argv[2];

	close(pipes->fd[1]);
	if (dup2(pipes->fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipes->fd[0]);
	path = find_command_path(argv[0], garbage);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(argv[0], garbage);
	new_argv[1] = NULL;
	execve(path, new_argv, envp);
	exit(EXIT_FAILURE);
}

int	ft_count(t_command *command, int *i)
{
	int	size;

	size = 0;
	while (command[*i + size].type && command[*i + size].type == WORD)
		size++;
	return (size);
}

char	**create_cmd_args(t_command *command, int *i, t_alloc *garbage)
{
	char	**cmd_args;
	int		j;

	j = 0;
	cmd_args = garb_malloc(sizeof(char *), ft_count(command, i) + 1, &garbage);
	if (!cmd_args)
		return (NULL);
	while (command[*i].type == WORD || command[*i].type == CODE)
	{
		cmd_args[j] = ft_strjoin(cmd_args[j], command[*i].word, garbage);
		printf("args = %s\n", cmd_args[j]);
		if (!cmd_args[j])
			return (NULL);
		*i += 1;
		j++;
	}
	return (cmd_args);
}

void	handle_command(char *input, t_code *code, char **envp, t_alloc *garbage)
{
	t_command	*command;
	t_pipe		pipes;
	char		**cmd_args;
	int			i;
	int			exec;

	i = 0;
	exec = 0;
	command = ft_parsing(input, garbage);
	if (command == NULL)
		return ;
	cmd_args = NULL;
	while (command[i].type != 0)
	{
		if (command[i].type == WORD || command[i].type == 0 || command[i].type == CODE)
			cmd_args = create_cmd_args(command, &i, garbage);
		if (command[i].type == PIPE)
		{
			ft_multipipes(command, garbage, envp, cmd_args, &i, code);
			exec++;
		}
		if (command[i].type >= LEFT_CHEV && command[i].type <= DB_RIGHT_CHEV)
			exec = init_redirection(command, &i, cmd_args, envp, code);
		if (command[i].type == DB_LEFT_CHEV)
		{
			heredoc(command[i + 1].word, &pipes, cmd_args, envp, garbage);
		}
		if (cmd_args != NULL && exec == 0)
		{
			if (execute_builtins(cmd_args, envp, code, garbage) == -1)
				execute_non_builtin(envp, code, cmd_args, garbage);
		}
		else
			i++;
	}
}




// void	handle_command(char *input, t_code *code, char **argv, char **envp)
// {
//     t_command *command;
//     t_pipe pipes;
//     t_quotes quotes = {FALSE, FALSE, FALSE, 0, NULL};
//     char **cmd_args;
//     int i = 0;


//     command = get_command(input, &quotes);

// 	while (command[i].type != 0)
// 	{
// 		if (command[i].type == PIPE)
// 		{
//             int cmd_start = i;
//             while (command[i].type == WORD)
//                 i++;
// 			if (command[i].type >= LEFT_CHEV && 
// 				command[i].type <= DB_RIGHT_CHEV && command[i + 1].type == WORD)
// 			{
// 				// cmd_args = create_cmd_args(command + cmd_start);
// 				// printf("cmd_args created: %s\n", cmd_args[0]);
// 				// if (command[i + 1].type == WORD)
// 				// {
// 					handle_redirection(code, command, argv, envp);
// 					i += 2;
// 				// }
// 			}
//             else if (command[i].type == PIPE)
// 			{
//                 split_command_for_pipes(input, &pipes);
//                 execute_pipe(&pipes, envp, code);
//                 break;
//             }
//             else
// 			{
//                 cmd_args = create_cmd_args(command + cmd_start);
//                 if (execute_builtins(cmd_args, envp, code) == -1) {
//                     execute_non_builtin(envp, code, cmd_args);
//                 }
//                 i = cmd_start;
//             }
//             free_parsed_command_line(cmd_args);
//         } 
//         i++; 
//     }
//     ft_free_command(command);
// }





