/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/07 17:22:46 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_code				*code;
	char				*line;
	struct sigaction	sa;
	struct sigaction	sq;

	if (argc != 1)
	{
		printf("Error arg : no argument required\n");
		return (1);
	}
	code = malloc(sizeof(t_code));
	if (!code)
		return (1);
	code->code_status = 0;
	if (init_sigactionsa(&sa) == -1)
		return (1);
	if (init_sigactionsq(&sq) == -1)
		return (1);
	while (1)
	{
		line = readline("minishell > ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != 0)
		{
			add_history(line);
			handle_command(line, code, argv, envp);
		}
	}
	clear_history();
	free (code);
	return (0);
}
