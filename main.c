/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/22 19:32:49 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_code	*code;
	char	*line;
	char	*prompt;

	code = malloc(sizeof(t_code));
	if (!code)
		return (1);
	code->code_status = 0;
	while (1)
	{
		prompt = "minishell > ";
		line = readline(prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != 0)
		{
			printf("%s\n", line);
			add_history(line);
			handle_command(line, code, envp);
		}
	}
	clear_history();
	free (code);
	return (0);
}
