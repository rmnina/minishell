/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 19:14:54 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/25 20:19:18 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	generate_temp_filename(t_minishell **main)
{
	int		fd;
	char	random_string[11];
	char	buffer;
	int		i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		perror("Error opening /dev/urandom");
	i = 0;
	buffer = 0;
	while (i < 10)
	{
		read(fd, &buffer, 1);
		random_string[i] = (buffer % 26) + 'a';
		++i;
	}
	random_string[10] = '\0';
	close(fd);
	(*main)->tmp_filename = ft_strjoin("/tmp/heredoc_", random_string);
	if (!(*main)->tmp_filename)
		exit(EXIT_FAILURE);
}

void	init_temp_file(t_minishell **main)
{
	generate_temp_filename(main);
	(*main)->tmp_fd = open((*main)->tmp_filename, \
	O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if ((*main)->tmp_fd < 0)
		exit(EXIT_FAILURE);
}

char	**get_delimiter(t_minishell **main, int *i, t_alloc **garbage)
{
	int		j;
	int		size;

	j = 0;
	size = 0;
	while ((*main)->command[*i + j].type == DB_LEFT_CHEV)
	{
		j += 2;
		size++;
	}
	j = 0;
	(*main)->h_delimiter = garb_malloc(sizeof(char *), size + 1, EXEC, garbage);
	while ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		(*i)++;
		(*main)->h_delimiter[j] = ft_g_strdup((*main)->command[*i].word, \
		EXEC, garbage);
		if (!(*main)->h_delimiter[j])
			return (NULL);
		j++;
		(*i)++;
	}
	return ((*main)->h_delimiter);
}

void	read_add(t_minishell **main, int *j, t_alloc **garbage)
{
	int	line_count;

	line_count = 0;
	while (42)
	{
		line_count++; 
		(*main)->h_line = readline("> ");
		if (!(*main)->h_line)
		{
			printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted `%s')\n", line_count, (*main)->h_delimiter[*j]);
			break ;
		}
		else if (ft_strcmp((*main)->h_line, (*main)->h_delimiter[*j]) == 0)
		{
			free((*main)->h_line);
			break ;
		}
		if (heredoc_is_expand((*main)->h_line))
			(*main)->h_line = heredoc_get_expand(main, garbage);
		write((*main)->tmp_fd, (*main)->h_line, ft_strlen((*main)->h_line));
		write((*main)->tmp_fd, "\n", 1);
		free((*main)->h_line);
	}
}

int	ft_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	int	j;

	j = 0;
	(*main)->h_delimiter = get_delimiter(main, i, garbage);
	while ((*main)->h_delimiter[j])
	{	
		init_temp_file(main);
		read_add(main, &j, garbage);
		j++;
		close((*main)->tmp_fd);
	}
	open((*main)->tmp_filename, O_RDONLY);
	if ((*main)->tmp_fd < 0)
		exit(EXIT_FAILURE);
	dup2((*main)->tmp_fd, STDIN_FILENO);
	close((*main)->tmp_fd);
	return (1);
}
