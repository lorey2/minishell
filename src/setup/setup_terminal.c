/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_terminal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:09:19 by lorey             #+#    #+#             */
/*   Updated: 2025/04/01 16:09:32 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_terminal(t_term *term)
{
	tcgetattr(STDIN_FILENO, &term->original);
	term->modified = term->original;
	term->modified.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term->modified);
}

void	restore_terminal(t_term *term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &term->original);
}
