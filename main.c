/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/08 19:30:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_alloc	init_garbage(t_alloc *garbage)
{
	garbage->adr = NULL;
	garbage->count = 0;
}


int	main(int argc, char **argv, char **envp)
{
	t_code		*code;
	char		*line;
	t_alloc		garbage;

	(void)argv;
	if (argc != 1)
	{
		printf("Error arg : no argument required\n");
		return (1);
	}
	code = malloc(sizeof(t_code));
	if (!code)
		return (1);
	code->code_status = 0;
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
			handle_command(line, code, envp, &garbage);
		}
	}
	clear_history();
	free (code);
	return (0);
}
