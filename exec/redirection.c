/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/04 18:43:24 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redirection_type(t_command *exec, char *symbol, char *file)
{
	if (!ft_strcmp(symbol, "<<"))
		exec->redirection_type = REDIRECT_APPEND_INPUT;
	else if (!ft_strcmp(symbol, ">"))
		exec->redirection_type = REDIRECT_OUTPUT;
	else if (!ft_strcmp(symbol, ">>"))
		exec->redirection_type = REDIRECT_APPEND_OUTPUT;
	else if (!ft_strcmp(symbol, "<"))
		exec->redirection_type = REDIRECT_INPUT;
	exec->redirection_file = file;
}

void	redir_symbol(t_command *exec, char **cmd_args)
{
	int	i;

	i = 0;
	exec->word = cmd_args[0];
	exec->redirection_file = NULL;
	exec->redirection_type = NO_REDIRECTION;

	while (cmd_args[i] != NULL)
	{
		if (!ft_strcmp(cmd_args[i], "<<") || !ft_strcmp(cmd_args[i], ">") || \
			!ft_strcmp(cmd_args[i], ">>") || !ft_strcmp(cmd_args[i], "<"))
		{
			set_redirection_type(exec, cmd_args[i], cmd_args[i + 1]);
			cmd_args[i] = NULL;
			if (cmd_args[i + 1] != NULL)
				cmd_args[i + 1] = NULL;
			break ;
		}
		i++;
	}
}

void	execute_redirection(t_command *exec, char **argv, char **envp)
{
	int		fd;
	int		flags;
	char	*path;

	if (exec->redirect_type == REDIRECT_OUTPUT || \
		exec->redirect_type == REDIRECT_APPEND_OUTPUT)
	{
		if (exec->redirect_type == REDIRECT_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (exec->redirect_type == REDIRECT_APPEND_OUTPUT)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(exec->file, flags, 0644);
		dup2(fd, STDOUT_FILENO);
	}
	else if (exec->redirect_type == REDIRECT_INPUT)
	{
		fd = open(exec->file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
	}
	close(fd);
	path = find_command_path(exec->word);
	execve(path, argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	init_exec_struct(t_command *exec)
{
	exec->file = exec->redirection_file;
	exec->redirect_type = exec->redirection_type;
}

int	handle_redirection(t_code *code, char *input, char **argv, char **envp)
{
	t_pipe		pipes;
	t_command	exec;
	char		**cmd_args1;
	int			hd_status;

	(void)argv;
	hd_status = 0;
	cmd_args1 = init_parsing(input);
	redir_symbol(&exec, cmd_args1);
	if (exec.redirection_type == REDIRECT_APPEND_INPUT)
	{
		hd_status = heredoc(exec.redirection_file, &pipes, cmd_args1, envp);
		code->code_status = hd_status;
		free_parsed_command_line(cmd_args1);
		return (1);
	}
	else if (exec.redirection_type != NO_REDIRECTION)
	{
		init_exec_struct(&exec);
		pid_redir(&exec, cmd_args1, envp, code);
		free_parsed_command_line(cmd_args1);
		return (1);
	}
	free_parsed_command_line(cmd_args1);
	return (0);
}

