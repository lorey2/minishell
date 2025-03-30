/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:31:25 by maambuhl          #+#    #+#             */
/*   Updated: 2025/03/30 20:32:33 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipe(t_data *data)
{
	t_parsing_data	*token;
	int				i;

	i = 0;
	token = data->token;
	while (token)
	{
		if (token->pipe)
			i++;
		token = token->next;
	}
	return (i);
}

int	open_in_file(char *file_name, t_data *data)
{
	int	fd;

	fd = open(file_name, O_RDONLY, 0666);
	if (!fd)
		error("Cannot open file", data);
	return (fd);
}

int	open_file(t_file *file, t_data *data)
{
	int	fd;

	if (file->append)
		fd = open(file->name, O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fd = open(file->name, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fd == -1)
		error("Cannot open file", data);
	return (fd);
}

t_parsing_data	*get_last_token(t_parsing_data *token)
{
	t_parsing_data	*head;

	head = token;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

void	free_heres(t_parsing_data *token)
{
	t_here_docs	*here;
	t_here_docs	*tmp;

	here = token->here_docs;
	while (here)
	{
		safe_free((void **)&here->delimiter);
		tmp = here;
		here = here->next;
		safe_free((void **)&tmp);
	}
}
