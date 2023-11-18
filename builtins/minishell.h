/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:20:35 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 11:41:05 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

typedef struct EnvVar {
    char *name; // le nom de la variable d'environnement
    char *value; // la valeur de la variable d'environnement
    struct EnvVar *next;
} EnvVar;

EnvVar *env_vars = NULL;
extern char **environ;

#endif 
