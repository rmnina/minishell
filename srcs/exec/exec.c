/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/08 23:59:35 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtins(char **cmd_args, char ***envp, t_code *code, t_alloc *garbage)
{
	if (cmd_args[0] == NULL)
		return (0);
	printf("Exécution de la commande intégrée: %s\n", cmd_args[0]);
	if (ft_strcmp(cmd_args[0], "cd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "echo") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("echo"))
		return (ft_echo(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "env") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("env"))
		return (ft_env(*envp, code));
	if (ft_strcmp(cmd_args[0], "exit") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("exit"))
		return (ft_exit(cmd_args, code, garbage));
	if (ft_strcmp(cmd_args[0], "export") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("export"))
		return (ft_export(envp, cmd_args, code, garbage));
	if (ft_strcmp(cmd_args[0], "pwd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("pwd"))
		return (ft_pwd(NULL, NULL, code));
	if (ft_strcmp(cmd_args[0], "unset") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("unset"))
		return (ft_unset(envp, cmd_args + 1, code));
	return (-1);
}


int	execute_non_builtin(char ***envp, t_code *code, char **cmd_args, t_alloc *garbage)
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
		init_sigquit();
		printf("Exécution de la commande non intégrée: %s\n", cmd_args[0]);
		execute_command(cmd_args, envp, garbage);
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

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0 ||
		ft_strcmp(command, "echo") == 0 ||
		ft_strcmp(command, "env") == 0 ||
		ft_strcmp(command, "exit") == 0 ||
		ft_strcmp(command, "export") == 0 ||
		ft_strcmp(command, "pwd") == 0 ||
		ft_strcmp(command, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

void	heredoc_child(t_pipe *pipes, char **argv, char ***envp, t_code *code, t_alloc *garbage)
{
	char	*path;
	char	*new_argv[2];

	path = NULL;
	close(pipes->fd[1]);
	if (dup2(pipes->fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipes->fd[0]);
	if (!argv[0] || !*argv)
		exit (EXIT_FAILURE);
	if (is_builtin(argv[0]))
		execute_builtins(argv, envp, code, garbage);
	else
	{
		path = find_command_path(argv[0], garbage);
		if (!path)
		{
			perror("path");
			exit(EXIT_FAILURE);
		}
		new_argv[0] = ft_strdup(argv[0], garbage);
		if (!new_argv[0])
			exit (EXIT_FAILURE);
		new_argv[1] = NULL;
		execve(path, new_argv, *envp);
		perror("execeve failed");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
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
	int		num_args;
	int		j;

	j = 0;
	cmd_args = NULL;
	num_args = ft_count(command, i);
	cmd_args = garb_malloc(sizeof(char *), num_args + 1, &garbage);
	if (!cmd_args)
		return (NULL);
	while (command[*i].type == WORD || command[*i].type == CODE)
	{
		cmd_args[j] = NULL;
		cmd_args[j] = ft_strjoin(cmd_args[j], command[*i].word, garbage);
		if (!cmd_args[j])
			return (NULL);
		*i += 1;
		j++;
	}
	cmd_args[j] = NULL;
	return (cmd_args);
}

int count_commands(t_command *command) {
    int count = 0;
    int i = 0;

    while (command[i].type != 0) {
        if (command[i].type == WORD) {
            count++;
            // Sauter tous les mots suivants qui font partie de la même commande
            while (command[i].type == WORD) {
                i++;
            }
        } else {
            i++;
        }
    }
    return count;
}


void	handle_command(char *input, t_code *code, char ***envp, t_alloc *garbage)
{
	t_command	*command;
	char		**cmd_args;
	int			i;
	int			exec;

	i = 0;
	exec = 0;
	cmd_args = NULL;
	command = ft_parsing(input, garbage, envp);
	int num_commands = count_commands(command);
	if (command == NULL)
		return ;
	while (command[i].type != 0)
	{
		if (command[i].type == WORD || command[i].type == 0 || command[i].type == CODE)
			cmd_args = create_cmd_args(command, &i, garbage);
		if (command[i].type == PIPE)
		{
			printf("command : %s\n", command->word);
			//ft_multipipes(command, garbage, envp, cmd_args, &i, code);
			execute_pipeline(command, num_commands, envp, code, garbage);
			exec++;
		}
		if (command[i].type >= LEFT_CHEV && command[i].type <= DB_LEFT_CHEV)
			exec = init_redirection(command, &i, cmd_args, envp, code);
		if (cmd_args != NULL && exec == 0)
		{
			if (execute_builtins(cmd_args, envp, code, garbage) == -1)
				execute_non_builtin(envp, code, cmd_args, garbage);
		}
		else
			i++;
	}
}





