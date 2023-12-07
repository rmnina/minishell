/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/07 19:15:25 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtins(char **cmd_args, char **envp, t_code *code)
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
		return (ft_exit(cmd_args, code));
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

void	execute_non_builtin(char **envp, t_code *code, char **cmd_args)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror ("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execute_command(cmd_args, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code->code_status = WEXITSTATUS(status);
	}
}

void	heredoc_child(t_pipe *pipes, char **argv, char **envp)
{
	char	*path;
	char	*new_argv[2];

	close(pipes->pipefd[1]);
	if (dup2(pipes->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipes->pipefd[0]);
	path = find_command_path(argv[0]);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(argv[0]);
	new_argv[1] = NULL;
	execve(path, new_argv, envp);
	exit(EXIT_FAILURE);
}

// void	handle_command(char *input, t_code *code, char **argv, char **envp)
// {
// 	char		**cmd_args;
// 	t_pipe		pipes;

// 	if (ft_strcmp(input, "$?") == 0)
// 	{
// 		execute_status_builtin(code);
// 		return ;
// 	}
// 	if (commands_with_pipes_detected(input))
// 	{
// 		split_command_for_pipes(input, &pipes);
// 		execute_pipe(&pipes, envp, code);
// 	}
// 	else
// 	{
// 		cmd_args = init_parsing(input);
// 		if (handle_redirection(code, input, argv, envp))
// 		{
// 			free_parsed_command_line(cmd_args);
// 			return ;
// 		}
// 		if (execute_builtins(cmd_args, envp, code) == -1)
// 			execute_non_builtin(input, envp, code);
// 		free_parsed_command_line(cmd_args);
// 	}
// }

// char	**create_cmd_args(t_command *command, int word_count)
// {
// 	int		word_count;
// 	int		i;
// 	char	**cmd_args;

// 	word_count = 0;
// 	while (command[word_count].type == WORD)
// 	{
// 		word_count++;
// 	}
// 	cmd_args = malloc(sizeof(char *) * (word_count + 1));
// 	if (!cmd_args)
// 	{
// 		perror("Allocation error in create_cmd_args");
// 		exit(EXIT_FAILURE);
// 	}
// 	i = 0;
// 	while (i < word_count)
// 	{
// 		cmd_args[i] = ft_strdup(command[i].word);
// 		if (!cmd_args[i])
// 		{
// 			perror("Allocation error in create_cmd_args");
// 			exit(EXIT_FAILURE);
// 		}
// 		i++;
// 	}
// 	cmd_args[word_count] = NULL;
// 	return (cmd_args);
// }

char **create_cmd_args(t_command *command, int word_count)
{
    char **cmd_args = malloc(sizeof(char *) * (word_count + 1));
    if (!cmd_args)
    {
        perror("Allocation error in create_cmd_args");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < word_count; i++)
    {
        cmd_args[i] = ft_strdup(command[i].word);
        if (!cmd_args[i])
        {
            perror("Allocation error in create_cmd_args");
            exit(EXIT_FAILURE);
        }
    }
    cmd_args[word_count] = NULL;

    return cmd_args;
}

// void	handle_command(char *input, t_code *code, char **argv, char **envp)
// {
// 	t_command	*command;
// 	t_quotes	quotes = {FALSE, FALSE, FALSE};
// 	t_expand	expand = {0, 0, FALSE};
// 	t_pipe		pipes;
// 	char		**cmd_args;
// 	int			i;

// 	i = 0;
// 	command = get_command(input, &quotes, &expand);
// 	ft_error_lexer(command);
// 	if (ft_strcmp(input, "$?") == 0)
// 	{
// 		execute_status_builtin(code);
// 		return ;
// 	}
// 	cmd_args = create_cmd_args(command);
// 	while (command[i].type != 0)
// 	{
// 		if (command->type == WORD)
// 		{
// 			cmd_args = create_cmd_args(command + i);
// 			if (execute_builtins(cmd_args, envp, code) == -1)
// 				execute_non_builtin(envp, code, cmd_args);
// 			free_parsed_command_line(cmd_args);
// 			while (command[i].type == WORD && command[i].type != 0)
// 				i++;
// 		}
// 		else if (command[i].type == PIPE)
// 		{
// 			split_command_for_pipes(input, &pipes);
// 			execute_pipe(&pipes, envp, code);
// 			break ;
// 		}
// 		else if (command[i].type == LEFT_CHEV || 
// 			command[i].type == RIGHT_CHEV || command[i].type == DB_LEFT_CHEV 
// 			|| command[i].type == DB_RIGHT_CHEV)
// 		{
// 			handle_redirection(code, command + i, argv, envp);
// 			while (command[i].type >= LEFT_CHEV && command[i].type <= DB_RIGHT_CHEV)
// 				i++;
// 		}
// 		else
// 			i++;
// 	}
// 	ft_free_command(command);
// }



// void handle_command(char *input, t_code *code, char **argv, char **envp)
// {
//     t_command *command;
//     t_quotes quotes = {FALSE, FALSE, FALSE, 0, NULL};
//     t_pipe pipes;
//     char **cmd_args;
//     int i = 0;
	
//     command = get_command(input, &quotes);
//     ft_error_lexer(command);
//     while (command[i].type != 0)
//     {
// 		if (command[i + 1].type == PIPE)
//         {
// 			split_command_for_pipes(input, &pipes);
// 			execute_pipe(&pipes, envp, code);
// 			break ;
//         }
//         else if (command[i].type == WORD)
//         {
//             cmd_args = create_cmd_args(command + i);
//             if (command[i + 1].type >= LEFT_CHEV && command[i + 1].type <= DB_RIGHT_CHEV)
//             {
//                 printf("Redirection detected. Type: %d\n", command[i + 1].type);
// 				handle_redirection(code, command + i + 1, argv, envp);
//                 while (command[i].type != WORD && command[i].type != 0)
// 					i++;
// 				continue;
//             }
//             if (execute_builtins(cmd_args, envp, code) == -1)
// 			{
//                 printf("Executing non-builtin command: %s\n", cmd_args[0]);
// 				execute_non_builtin(envp, code, cmd_args);
// 			}
//             free_parsed_command_line(cmd_args);
//             while (command[i].type == WORD && command[i].type != 0)
//                 i++;
//         }
//         else
//         {
//             i++;
//         }
//     }
//     ft_free_command(command);
// }

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

void handle_command(char *input, t_code *code, char **envp)
{
    t_command *command = get_command(input, &(t_quotes){FALSE, FALSE, FALSE, 0, NULL});
    ft_error_lexer(command);

    for (int i = 0; command[i].type != 0; i++)
    {
        if (command[i].type == WORD)
        {
            int redirIndex = i;
            while (command[redirIndex].type == WORD)
                redirIndex++;

            char **cmd_args = create_cmd_args(command + i, redirIndex - i);

            if (command[redirIndex].type >= LEFT_CHEV && command[redirIndex].type <= DB_RIGHT_CHEV)
            {
                handle_redirection(code, command + redirIndex, cmd_args, envp);
            }
            else if (execute_builtins(cmd_args, envp, code) == -1)
            {
                execute_non_builtin(envp, code, cmd_args);
            }
            free_parsed_command_line(cmd_args);

            i = redirIndex - 1; // Ajuster l'index pour sauter les éléments de redirection
        }
    }

    ft_free_command(command);
}



