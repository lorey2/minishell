/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:30:27 by maambuhl          #+#    #+#             */
/*   Updated: 2025/04/01 18:33:03 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_out_file(t_parsing_data *token, t_data *data)
{
	t_file	*file;

	file = token->outfile_list;
	if (token->outfile)
	{
		while (file)
		{
			if (!*file->name)
			{
				ft_putstr_fd("Synthax error\n", STDERR_FILENO);
				return (-1);
			}
			token->fd_out = open_file(file, data);
			file = file->next;
		}
		return (1);
	}
	return (0);
}

int	check_in_file(t_parsing_data *token, t_data *data)
{
	t_file	*file;

	file = token->infile_list;
	while (file)
	{
		if (access(file->name, F_OK) != 0)
		{
			ft_putstr_fd(file->name, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (-1);
		}
		file = file->next;
	}
	if (token->infile)
	{
		token->fd_in = open_in_file(token->infile, data);
		dup2(token->fd_in, STDIN_FILENO);
		close(token->fd_in);
		return (1);
	}
	else if (token->here)
		return (here_doc(token, data), 1);
	return (0);
}

int	check_file(t_parsing_data *token, t_data *data, int saved_stdin)
{
	if (check_in_file(token, data) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (0);
	}
	if (check_out_file(token, data) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
		return (0);
	}
	return (1);
}
