/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:21:08 by lorey             #+#    #+#             */
/*   Updated: 2025/01/17 14:45:21 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <signal.h>

/* ************************************************************************** */
/* setup CTRL-C. It makes a new line and rewrite the prompt                   */
/* no error possible                                                          */
/* list of malloc at this point : path_data->path->split and a lot more later */
/* added malloc : nothing                                                     */
/* ************************************************************************** */

void	sigint_handler(int sig __attribute__((unused)))
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line ();
	rl_redisplay();
}

/* ************************************************************************** */
/* setup CTRL-\. It does nothing but the prompt as to be redisplayed          */
/* no error possible                                                          */
/* list of malloc at this point : path_data->path->split and a lot more later */
/* added malloc : nothing                                                     */
/* ************************************************************************** */

void	sigquit_handler(int sig __attribute__((unused)))
{
	rl_on_new_line();
	rl_redisplay();
}

/* ************************************************************************** */
/* setup the signals ctrl-c ctrl-\ ctrl-d                                     */
/* no error possible                                                          */
/* list of malloc at this point : path_data->path->split                      */
/* added malloc : nothing                                                     */
/* ************************************************************************** */

void	setup_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// WE DONT NEED TO HANDLE CTRL D FOR NOW. THE BASIC CTRL D BREAK THE LOOP WITH /
// CURRENT CODE                                                                /
////////////////////////////////////////////////////////////////////////////////

//TODO	NOW THE FUNCTION EXITS BUT NOTHING IS FREED
//		INSTEAD OF JUST EXITING EXITING
//		WE HAVE TO SETUP A GLOBAL VARIABLE AND EXIT FROM THE BIG LOOP
//TODO2	IF WE CTRL-D WITH PING, MINISHELL ENDS BUT PING CONTINUES AS A ZOMBIE
//		WE WILL HAVE TO USE KILL
/* ************************************************************************** */
/* setup CTRL-D. It exits the program                                         */
/* no error possible                                                          */
/* list of malloc at this point : path_data->path->split and a lot more later */
/* added malloc : nothing                                                     */
/* ************************************************************************** */
/*
void	sigterm_handler(int sig __attribute__((unused)))
{
	write(1, "lkjhgfdsa", 9);
	g_signal = 1;
}
//for now nothing is required for ctrl/c
//but if we need it anyway here is the code :
	struct sigaction	sa_term;
	sa_term.sa_handler = sigterm_handler;
	sa_term.sa_flags = 0;
	sigemptyset(&sa_term.sa_mask);
	sigaction(SIGTERM, &sa_term, NULL);
*/
////////////////////////////////////////////////////////////////////////////////
// END OF CTRL D CODE                                                          /
////////////////////////////////////////////////////////////////////////////////
