/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/14 13:02:33 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int    init_sigactionsa(struct sigaction *sa)
{
    ft_bzero(sa, sizeof(struct sigaction));
    sa->sa_handler = sigint_handler;
    sa->sa_flags = 0;
    sigaction(SIGINT, sa, NULL);
    signal(SIGQUIT, SIG_IGN);
    return (0);
}

int    init_sigquit(void)
{
    struct sigaction    quit;

    sigemptyset(&quit.sa_mask);
    quit.sa_handler = sigint_handler;
    quit.sa_flags = 0;
    sigaction(SIGQUIT, &quit, NULL);
    return (0);
}

void    child_handler(int signum)
{
    if (signum == SIGINT)
        printf("\n");
    if (signum == SIGQUIT)
        printf("Quit (core dumped)\n");
}

int    process_prompt(void)
{
    struct sigaction    prompt;

    ft_bzero(&prompt, sizeof(prompt));
    prompt.sa_handler = child_handler;
    prompt.sa_flags = 0;
    sigaction(SIGINT, &prompt, NULL);
    sigaction(SIGQUIT, &prompt, NULL);

    return (0);
}

void    sigint_handler(int signum)
{
    if (signum == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_redisplay();
    }
}