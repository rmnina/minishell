/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/22 14:48:03 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	g_sigint = 0;


int	main(int argc, char **argv, char **envp)
{
	t_code				*code;
	char				*line;
	t_alloc				*garbage;
	struct sigaction	sa;

	(void)argv;
	code = NULL;
	line = NULL;
	garbage = NULL;
	if (argc != 1)
	{
		printf("Error arg : no argument required\n");
		return (1);
	}
	code = garb_malloc(sizeof(t_code), 1, &garbage);
	if (!code)
		return (1);
	code->code_status = 0;
	if (init_sigquit() == -1)
		return(-1);
	while (1)
	{
		if (init_sigactionsa(&sa) == -1)
			return (1);
		line = readline("minishell > ");
		if (line == NULL)
		{
			printf("exit\n");
			if (garbage)
				free_garbage(&garbage, 0);
			break ;
		}
		if (line[0] != 0)
		{
			add_history(line);
			handle_command(line, code, envp, garbage);
		}
		free(line);
	}
	clear_history();
	if (garbage)
		free_garbage(&garbage, 0);
	return (0);
}