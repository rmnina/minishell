/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/18 16:31:02 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_sigint = 0;


int	main(int argc, char **argv, char **envp)
{
	t_code				*code;
	char				*line;
	t_alloc				*garbage;
	struct sigaction	sa;
	struct sigaction	sq;

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
	if (init_sigactionsa(&sa) == -1)
		return (1);
	if (init_sigactionsq(&sq) == -1)
		return (1);
	while (1)
	{
		//printf("Attente d'entrée utilisateur...\n");
		if (g_sigint == 0)
		{
			line = readline("minishell > ");
		}	
		//printf("readline a retourne: %s\n", line);
		if (!line)
		{
			printf("exit\n");
			//printf("EOF détecté (Ctrl-D), sortie du shell.\n");
			if (garbage)
				free_garbage(&garbage, 0);
			break ;
		}
		//printf("Ligne reçue: %s\n", line); 
		if (line[0] != 0)
		{
			add_history(line);
			handle_command(line, code, envp, garbage);
		}
	}
	clear_history();
	return (0);
}