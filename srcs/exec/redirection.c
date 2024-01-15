/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/15 17:57:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	get_all_redir(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_output(main, i))
	{
		while (is_output(main, i))
		{
			filename = ft_strdup((*main)->command[*i + 1].word, garbage);
			if (!filename)
				return (-1);
			(*main)->outfilefd = open(filename, O_CREAT | O_WRONLY, 0644);
			close((*main)->outfilefd);
			*i += 2;
		}
	}
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (!filename)
		return (-1);
	if (is_input(main, i))
	{
		(*main)->outfilefd = open(filename, O_CREAT | O_WRONLY, 0644);
		close((*main)->outfilefd);
		get_right_input(main, i, garbage);
	}
	if ((*main)->command[*i].type == RIGHT_CHEV)
		(*main)->outfilefd = redir_output(main, filename);
	else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->outfilefd = redir_append(main, filename);
	return (0);
}

int	get_right_input(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_input(main, i))
	{
		while (is_input(main, i))
		{
			filename = ft_strdup((*main)->command[*i + 1].word, garbage);
			if (((*main)->infilefd = open(filename, O_RDONLY, 0644)) == -1)
				break ;
			close((*main)->infilefd);
			*i += 2;
		}
	}
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (is_output(main, i))
	{
		if (!(((*main)->infilefd = open(filename, O_RDONLY, 0644)) == -1))
		{
			*i += 2;
			get_all_redir(main, i, garbage);
		}
	}
	if ((*main)->infilefd == -1 || !(((*main)->infilefd = open(filename, O_RDONLY, 0644)) == -1))
	{
		if (is_input(main, i) || is_output(main, i))
		{
			while (is_input(main, i) || is_output(main, i))
				*i += 2;
		}
	}
	else
		(*main)->infilefd = redir_input(main, filename);
	return (1);
}

void	handle_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	if ((*main)->command[*i].type == DB_RIGHT_CHEV || \
		(*main)->command[*i].type == RIGHT_CHEV)
	{
		if (get_all_redir(main, i, garbage) == -1)
			exit(EXIT_FAILURE);
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
		get_right_input(main, i, garbage);
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->infilefd = -2;
	(*main)->outfilefd = -2;
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		ft_heredoc(main, i, garbage);
		return (0);
	}
	handle_redirect(main, i, garbage);
	*i += 2;
	if ((*main)->infilefd == -1 || (*main)->outfilefd == -1)
	{
		write(2, "error : file could not be opened\n", 34);
		return(-1);
	}
	return (1);
}