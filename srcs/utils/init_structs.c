/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:43:20 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/23 20:05:21 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_minishell	*get_minishell(void)
{
	static t_minishell	main;

	return (&main);
}

void	configure_minishell(t_minishell *main, char **envp)
{
	main->redir = 0;
	main->status = -1;
	main->nb_cmd = 0;
	main->total_cmd = 0;
	main->tmp_filename = "/tmp/minishell.txt";
	main->pid = NULL;
	main->line = NULL;
	main->h_line = NULL;
	main->path = NULL;
	main->last_cd_path = NULL;
	main->cd_path = NULL;
	main->cmd_args = NULL;
	main->h_delimiter = NULL;
	main->envp = envp;
	main->command = NULL;
	main->parser = NULL;
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*main;

	main = get_minishell();
	main->fd_stdout = -1;
	main->fd_stdin = -1;
	main->fd[0] = -1;
	main->fd[1] = -1;
	main->old_fd = -1;
	main->infilefd = -1;
	main->outfilefd = -1;
	main->tmp_fd = -1;
	configure_minishell(main, envp);
	return (main);
}

void	restore_minishell(void)
{
	t_minishell	*main;

	main = get_minishell();
	main->redir = 0;
	main->fd[0] = -1;
	main->fd[1] = -1;
	main->fd_stdout = -1;
	main->fd_stdin = -1;
	main->tmp_fd = -1;
	main->infilefd = -1;
	main->outfilefd = -1;
	main->nb_cmd = 0;
	main->total_cmd = 0;
	main->old_fd = -1;
	main->status = -1;
	main->tmp_filename = "/tmp/minishell.txt";
	main->pid = NULL;
	main->line = NULL;
	main->h_line = NULL;
	main->path = NULL;
	main->cmd_args = NULL;
	main->h_delimiter = NULL;
	main->command = NULL;
	main->parser = NULL;
}
