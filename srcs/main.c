/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/09 17:01:09 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	g_sigint = 0;

int	init_main(t_code **code, t_alloc **garbage, int argc)
{
	if (argc != 1)
	{
		write(2, "Error arg : no argument required\n", 34);
		return (1);
	}
	*code = garb_malloc(sizeof(t_code), 1, garbage);
	if (!*code)
		return (1);
	(*code)->code_status = 0;
	// if (init_sigquit(&quit) == -1)
	// 		return (1);
	return (0);
}

int	ft_minishell(char *line, t_code *code, char **envp, t_alloc **garbage)
{
	struct sigaction	sa;

	while (1)
	{
		if (init_sigactionsa(&sa) == -1)
			return (1);
		line = readline("minishell > ");
		if (line == NULL)
		{
			printf("exit\n");
			if (garbage)
				free_garbage(garbage, 0);
			break ;
		}
		if (line[0] != 0)
		{
			add_history(line);
			handle_command(line, code, &envp, garbage);
			if (code->code_status == SPECIAL_EXIT_CODE)
				break ;
		}
		if (line)
			free(line);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_code		*code;
	char		*line;
	t_alloc		*garbage;
	int			exit_status;

	(void)argv;
	code = NULL;
	line = NULL;
	garbage = NULL;
	exit_status = 0;
	if (init_main(&code, &garbage, argc) == 1)
		return (1);
	exit_status = ft_minishell(line, code, envp, &garbage);
	clear_history();
	if (garbage)
		free_garbage(&garbage, 0);
	return (exit_status);
}
