/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/14 17:07:35 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_code		*code;
	char		*line;
	t_alloc		*garbage;

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
	while (2 + 2 == 4)
	{
		line = readline("minishell > ");
		if (!line)
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
	}
	clear_history();
	return (0);
}