/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:42:47 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 11:22:38 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd() 
{
    char *cwd;
    
    cwd = getcwd(NULL, 0); // getcwd peut allouer la mémoire si vous passez NULL
    if (cwd != NULL) 
    {
        printf("%s\n", cwd);
        free(cwd); 
    } 
    else 
    {
        perror("pwd");
    }
}

int main(int ac, char **av)
{
        (void)ac;
        (void)av;
        
        pwd();
}

