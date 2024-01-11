/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 10:43:20 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/11 10:45:42 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_minishell	*get_minishell(void)
{
	static t_minishell	main;

	return (&main);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*main;
	
	main = get_minishell();
	main->pipe_fd[0] = -1;
	main->pipe_fd[1] = -1;
	main->pos = 0;
	main->pid = 0;
	main->line = NULL;
	main->h_line = NULL;
	main->path = NULL;
	main->last_cd_path = NULL;
	main->cd_path = NULL;
	main->cmd_args = NULL;
	main->h_delimiter = NULL;
	main->envp = envp;
	main->command = NULL;
	main->parser = NULL;
	return (main);
}

void	restore_minishell()
{
	t_minishell	*main;
	
	main = get_minishell();
	main->pipe_fd[0] = -1;
	main->pipe_fd[1] = -1;
	main->pos = 0;
	main->pid = 0;
	main->line = NULL;
	main->h_line = NULL;
	main->path = NULL;
	main->cmd_args = NULL;
	main->h_delimiter = NULL;
	main->command = NULL;
	main->parser = NULL;
}

t_parser	*get_parser(t_alloc **garbage)
{
	t_parser	*parser;

	parser = garb_malloc(sizeof(t_parser *), 1, garbage);
	return (parser);
}

t_parser	*init_parser(t_alloc **garbage)
{
	t_parser	*parser;

	parser = get_parser(garbage);
	parser->case_double = FALSE;
	parser->case_single = FALSE;
	parser->case_quotes = FALSE;
	parser->var = NULL;
	parser->vpos = 0;
	return (parser);
}
