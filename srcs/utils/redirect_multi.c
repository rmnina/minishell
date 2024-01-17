/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_multi.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 20:34:27 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/17 21:14:12 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int check_next_redir(t_minishell **main, int *i)
{
    int j;

    j = 1;
    while ((*main)->command[*i + j].type && (*main)->command[*i + j].type != PIPE)
    {
        if ((*main)->command[*i + j].type >= LEFT_CHEV \
		&& (*main)->command[*i + j].type <= DB_LEFT_CHEV)
			return (j);
		j++;
    }
    return (0);
}

int	browse_outputs(t_minishell **main, int *i, char **filename, t_alloc **garbage)
{
    while (is_output(main, i))
	{
		*filename = ft_strdup((*main)->command[*i + 1].word, garbage);
		if (!*filename)
			return (perror("filename alloc"), (*main)->code_status = 255, -1);
        dprintf(2, "filename browse = %s\n", *filename);
		if (((*main)->outfilefd = open(*filename, O_CREAT | O_WRONLY, 0644)) == -1)
			return (-1);
		close((*main)->outfilefd);
		if (check_next_redir(main, i) != 2)
		{
			dprintf(2, "WTF\n");
            *i += 2;
			while (check_redir(main, i) != 0)
			{
				(*main)->cmd_args = ft_strjoin_args(main, i, garbage);
				(*i)++;
			}
		}
		else
			*i += 2;
	}
	return (1);
}

int	browse_inputs(t_minishell **main, int *i, char **filename, t_alloc **garbage)
{
	while (is_input(main, i))
	{
		*filename = ft_strdup((*main)->command[*i + 1].word, garbage);
		if (!*filename)
			return (perror("filename alloc"), (*main)->code_status = 255, -1);
		if (((*main)->infilefd = open(*filename, O_RDONLY, 0644)) == -1)
			return (-1);
		close((*main)->infilefd);
		if (check_next_redir(main, i) != 2)
		{
			*i += 2;
			while (check_redir(main, i) != 0)
			{
				(*main)->cmd_args = ft_strjoin_args(main, i, garbage);
				(*i)++;
			}
		}
		else
			*i += 2;
	}
	return (1);
}