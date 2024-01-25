/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:35:46 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/25 11:10:30 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**create_cmd_args(t_minishell **main, int *i, t_alloc **garbage)
{
	char	**cmd_args;
	int		num_args;
	int		j;

	j = 0;
	cmd_args = NULL;
	num_args = ft_count((*main)->command, i);
	cmd_args = garb_malloc(sizeof(char *), num_args + 1, EXEC, garbage);
	if (!cmd_args)
		return (NULL);
	while ((*main)->command[*i].type == WORD)
	{
		cmd_args[j] = \
		ft_g_strjoin(cmd_args[j], (*main)->command[*i].word, EXEC, garbage);
		*i += 1;
		j++;
	}
	cmd_args[j] = NULL;
	return (cmd_args);
}

void	handle_command(t_minishell **main, t_alloc **garbage)
{
	int	i;

	i = 0;
	(*main)->cmd_args = NULL;
	(*main)->nb_cmd = 0;
	(*main)->command = ft_parsing(main, garbage);
	if ((*main)->command == NULL)
		return ;
	(*main)->cmd_args = create_cmd_args(main, &i, garbage);
	if (will_be_piped(main, &i))
	{
		(*main)->total_cmd = init_pids(main, garbage);
		ft_pipex(main, &i, garbage);
	}
	else if (check_redir(main, &i) != -1)
		init_redirect(main, &i, garbage);
	else
	{
		if (execute_builtins(main, garbage) == -1)
			execute_non_builtin(main, garbage);
	}
	if ((*main)->tmp_filename)
		unlink((*main)->tmp_filename);
}
