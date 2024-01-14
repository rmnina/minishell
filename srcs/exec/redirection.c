/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/14 17:09:04 by julietteand      ###   ########.fr       */
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
			(*main)->fd = open(filename, O_CREAT | O_WRONLY, 0644);
			close((*main)->fd);
			*i += 2;
		}
	}
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (!filename)
		return (-1);
	if (is_input(main, i))
	{
		(*main)->fd = open(filename, O_CREAT | O_WRONLY, 0644);
		close((*main)->fd);
		get_right_input(main, i, garbage);
	}
	if ((*main)->command[*i].type == RIGHT_CHEV)
		(*main)->fd = redir_output(filename);
	else if ((*main)->command[*i].type == DB_RIGHT_CHEV)
		(*main)->fd = redir_append(filename);
	return (0);
}

char	*get_right_input(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;

	filename = NULL;
	if (is_input(main, i))
	{
		while (is_input(main, i))
		{
			filename = ft_strdup((*main)->command[*i + 1].word, garbage);
			if (((*main)->fd = open(filename, O_RDONLY, 0644)) == -1)
				return (NULL);
			close((*main)->fd);
			*i += 2;
		}
	}
	filename = ft_strdup((*main)->command[*i + 1].word, garbage);
	if (is_output(main, i))
	{
		if (((*main)->fd = open(filename, O_RDONLY, 0644)) == -1)
				return (NULL);
		*i += 2;
		get_all_redir(main, i, garbage);
	}
	return (filename);
}

void	handle_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	char	*filename;
	
	filename = NULL;
	(*main)->fd = 0;
	if ((*main)->command[*i].type == DB_RIGHT_CHEV || \
		(*main)->command[*i].type == RIGHT_CHEV)
	{
		if (get_all_redir(main, i, garbage) == -1)
			exit(EXIT_FAILURE);
	}
	else if ((*main)->command[*i].type == LEFT_CHEV)
	{
		if ((filename = get_right_input(main, i, garbage)) == NULL)
		{
			if (is_input(main, i) || is_output(main, i))
			{
				while (is_input(main, i) || is_output(main, i))
					*i += 2;
			}
			(*main)->fd = -1;
		}
		else
			(*main)->fd = redir_input(filename);
	}
}

int	ft_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->fd = 0;
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		printf("Executing ft_heredoc: %s\n", (*main)->command[*i].word);
		ft_heredoc(main, i, garbage);
		return (0);
	}
	handle_redirect(main, i, garbage);
	*i += 2;
	if ((*main)->fd == -1)
	{
		write(2, "error : file couldnt be opened\n", 32);
		return (-1);
	}
	return (1);
}
