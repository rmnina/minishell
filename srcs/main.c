/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/25 00:18:53 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_sigstatus = 0;

int ft_minishell(t_minishell *main, t_alloc **garbage)
{

	while (1)
	{
		init_signal();
		main->line = readline(ANSI_COLOR_GREEN "minishell > " ANSI_COLOR_RESET);
		if (main->line == NULL)
		{
			printf("exit\n");
			if (garbage)
				free_garbage(garbage);
			break ;
		}
		if (main->line[0] != 0)
		{
			add_history(main->line);
			handle_command(&main, garbage);
		}
		if (main->line)
		{
			free(main->line);
			// free_small_garb(garbage);
		}
		if (g_sigstatus != 0)
			main->code_status = g_sigstatus;
		g_sigstatus = 0;
		restore_minishell();
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_minishell *main;
	t_alloc *garbage;
	int exit_status;

	(void)argv;
	garbage = NULL;
	exit_status = 0;
	if (argc != 1)
		return (write(2, "Error arg : no argument required\n", 34));
	if (!envp[0])
		envp = set_env(envp, &garbage);
	main = init_minishell(envp);
	if (main == NULL)
		return (2);
	exit_status = ft_minishell(main, &garbage);
	clear_history();
	if (garbage)
		free_garbage(&garbage);
	return (exit_status);
}
