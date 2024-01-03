/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/02 21:24:18 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtins(t_main_items *main, t_code *code, t_alloc *garbage)
{
	if (main->cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp(main->cmd_args[0], "cd") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(main->cmd_args, code));
	if (ft_strcmp(main->cmd_args[0], "echo") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("echo"))
		return (ft_echo(main->cmd_args, code));
	if (ft_strcmp(main->cmd_args[0], "env") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("env"))
		return (ft_env(main->envp, code));
	if (ft_strcmp(main->cmd_args[0], "exit") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("exit"))
		return (ft_exit(main->cmd_args, code, garbage));
	if (ft_strcmp(main->cmd_args[0], "export") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("export"))
		return (ft_export(main->envp, main->cmd_args, code, garbage));
	if (ft_strcmp(main->cmd_args[0], "pwd") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("pwd"))
		return (ft_pwd(NULL, NULL, code));
	if (ft_strcmp(main->cmd_args[0], "unset") == 0
		&& ft_strlen(main->cmd_args[0]) == ft_strlen("unset"))
		return (ft_unset(main->envp, main->cmd_args + 1, code));
	return (-1);
}


int	execute_non_builtin(t_main_items *main, t_code *code, t_alloc *garbage)
{
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
		execute_command(main->cmd_args, main->envp, garbage);
		exit(EXIT_FAILURE);
	}
	else
	{
		process_prompt();
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code->code_status = WEXITSTATUS(status);
	}
	return (-1);
}

void	heredoc_child(t_pipe *pipes, t_main_items *main, t_alloc *garbage)
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
	path = find_command_path(main->cmd_args[0], garbage);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(main->cmd_args[0], garbage);
	new_argv[1] = NULL;
	execve(path, new_argv, main->envp);
	perror("execeve failed");
	exit(EXIT_FAILURE);
}


int	ft_count(t_main_items *main, int *i)
{
	int	size;

	size = 0;
	while (main->command[*i + size].type && main->command[*i + size].type == WORD)
		size++;
	return (size);
}

char	**create_cmd_args(t_main_items *main, int *i, t_alloc *garbage)
{
	// char	**cmd_args;
	int		j;

	j = 0;
	main->cmd_args = garb_malloc(sizeof(char *), ft_count(main->command, i) + 1, &garbage);
	if (!main->cmd_args)
		return (NULL);
	while (main->command[*i].type == WORD || main->command[*i].type == CODE)
	{
		main->cmd_args[j] = ft_strjoin(main->cmd_args[j], main->command[*i].word, garbage);
		// printf("args = %s\n", cmd_args[j]);
		if (!main->cmd_args[j])
			return (NULL);
		*i += 1;
		j++;
	}
	return (main->cmd_args);
}

void	handle_command(t_main_items *main, t_code *code, t_alloc *garbage)
{
	// t_command	*command;
	// char		**cmd_args;
	int			i;
	int			exec;

	i = 0;
	exec = 0;
	main->command = ft_parsing(main->line, garbage, main->envp);
	if (main->command == NULL)
		return ;
	main->cmd_args = NULL;
	while (main->command[i].type != 0)
	{
		if (main->command[i].type == WORD || main->command[i].type == 0 || main->command[i].type == CODE)
			main->cmd_args = create_cmd_args(main->command, &i, garbage);
		if (main->command[i].type == PIPE)
		{
			ft_multipipes(main, &i, code, garbage);
			exec++;
		}
		if (main->command[i].type >= LEFT_CHEV && main->command[i].type <= DB_LEFT_CHEV)
			exec = init_redirection(main, &i, code);
		if (main->cmd_args != NULL && exec == 0)
		{
			if (execute_builtins(main, code, garbage) == -1)
				execute_non_builtin(main, code, garbage);
		}
		else
			i++;
	}
}





