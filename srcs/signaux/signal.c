/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariavillarroel <mariavillarroel@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 16:32:41 by mvillarr          #+#    #+#             */
/*   Updated: 2023/08/21 14:45:54 by mariavillar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

//c_lflag = 1011 0001      1011 0001
//ECHOCTL = 0000 0001 ~ -> 1111 1110  &
//          0000 0001      1011 0000
// clean ctr-c dans le terminal
void	set_termios(void)
{
	static struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag = term.c_lflag & ~ECHOCTL;
	tcsetattr(0, 0, &term);
}

// l.40 ctr-bck slash
// l.41 
void	ft_signal(void)
{
	struct sigaction	s;

	s.sa_handler = SIG_IGN;
	sigemptyset(&s.sa_mask);
	s.sa_flags = 0;
	sigaction(SIGQUIT, &s, NULL);
	s.sa_handler = sigint_manage;// function crl-c
	sigaction(SIGINT, &s, NULL);
}

void	sigint_manage(int num)
{
	(void)num;
	if (num == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	update_signal_handler(int num)
{
	(void)num;
	if (num == SIGINT)
	{
		write(1, "\n", 1);
		rl_redisplay();
	}
		else if (num == SIGQUIT)
	{
		write (1, "quitting minishell\n", 19);
		rl_redisplay();
	}
}

void	update_signal(void)
{
	struct sigaction	s;

	s.sa_flags = 0;
	s.sa_handler = update_signal_handler;
	sigemptyset(&s.sa_mask);
	sigaction(SIGQUIT, &s, NULL); //crt bcklash
	sigaction(SIGINT, &s, NULL); //crt C
}
