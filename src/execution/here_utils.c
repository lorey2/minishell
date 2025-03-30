/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:31:14 by maambuhl          #+#    #+#             */
/*   Updated: 2025/03/30 20:31:16 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	here_doc_write(t_parsing_data *token, int *pipefd)
{
	close(pipefd[0]);
	if (token->value)
		ft_putstr_fd(token->here, pipefd[1]);
	close(pipefd[1]);
}

void	here_doc(t_parsing_data *token, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		exit(0);
	pid = fork();
	if (pid == -1)
		exit(0);
	if (!pid)
	{
		here_doc_write(token, pipefd);
		free_everything(data);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], token->fd_in);
		close(pipefd[0]);
		wait(NULL);
	}
}
