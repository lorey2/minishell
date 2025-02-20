/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 19:57:47 by lorey             #+#    #+#             */
/*   Updated: 2025/02/19 14:56:56 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

/*
NAME
       env - run a program in a modified environment

SYNOPSIS
       env [OPTION]... [-] [NAME=VALUE]... [COMMAND [ARG]...]

DESCRIPTION
       Set each NAME to VALUE in the environment and run COMMAND.

       Mandatory arguments to long options are mandatory for short options too.

       -i, --ignore-environment
              start with an empty environment

       -0, --null
              end each output line with NUL, not newline

       -u, --unset=NAME
              remove variable from the environment

       -C, --chdir=DIR
              change working directory to DIR

       -S, --split-string=S
              process and split S into separate arguments;
			  used to pass multiple arguments on shebang lines

       --block-signal[=SIG]
              block delivery of SIG signal(s) to COMMAND

       --default-signal[=SIG]
              reset handling of SIG signal(s) to the default

       --ignore-signal[=SIG]
              set handling of SIG signals(s) to do nothing

       --list-signal-handling
              list non default signal handling to stderr

       -v, --debug
              print verbose information for each processing step

       --help display this help and exit

       --version
              output version information and exit

       A mere - implies -i.  If no COMMAND, print the resulting environment.

       SIG may be a signal name like 'PIPE', or a signal number like '13'.
	   Without SIG, all known signals are included.
	   Multiple signals can be comma-separated.
*/
// as you can see env can do a lot of things...
// For now it does only the madantory without any flags/args

#include "minishell.h"

void	env(t_env_data *e_data, t_parsing_data *p_data)
{
	int	i;

	i = -1;
	while (e_data->env[++i])
	{
		write(p_data->fd_out, e_data->env[i], ft_strlen(e_data->env[i]));
		write(p_data->fd_out, "\n", 1);
	}
}
