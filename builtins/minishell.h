/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:20:35 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/21 11:26:06 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <errno.h>
# include <unistd.h>
# include <sys/stat.h>
# include <string.h>

typedef struct s_EnvVar
{
	char			*name;
	char			*value;
	struct EnvVar	*next;
}	t_EnvVar;

t_EnvVar		*g_env_vars = NULL;
extern char		**environ;

#endif 
