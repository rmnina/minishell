/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 18:54:43 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_code	*code;
	char	*line;
	char	*prompt;
	char	**new_argv;

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
		prompt = "minishell > ";
		line = readline(prompt);
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
