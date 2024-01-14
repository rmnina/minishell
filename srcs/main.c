/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 13:12:16 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sigstatus = 0;

int	ft_minishell(t_minishell *main, t_alloc **garbage)
{
	struct sigaction	sa;
	
	while (1)
	{
		//init_signal();
		if (init_sigactionsa(&sa) == -1)
			return (1);
		main->line = readline("minishell > ");
		if (main->line == NULL)
		{
			printf("exit\n");
			if (garbage)
				free_garbage(garbage, 0);
			break ;
		}
		if (main->line[0] != 0)
		{
			add_history(main->line);
			handle_command(&main, garbage);
			// if (code->code_status == SPECIAL_EXIT_CODE)
			// 	break ;
		} 
		if (main->line)
			free(main->line);
		if (g_sigstatus != 0)
			main->code_status = g_sigstatus;
		g_sigstatus = 0;
		// if (garbage)
		// 	free_garbage(garbage, 0);
		restore_minishell();
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*main;
	t_alloc		*garbage;
	int			exit_status;

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
	// if (main->cd_path)
	// 	free(main->cd_path);
	// if (main->last_cd_path)
	// 	free(main->last_cd_path);
	if (garbage)
		free_garbage(&garbage, 0);
	return (exit_status);
}
