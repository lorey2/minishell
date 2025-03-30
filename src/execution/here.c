/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maambuhl <marcambuehl4@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 20:30:19 by maambuhl          #+#    #+#             */
/*   Updated: 2025/03/30 20:30:21 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*gnl(t_data *data)
{
	char	*buff;
	char	c;
	int		r;
	int		i;

	buff = safe_malloc(sizeof(char) * MAX_HERE_LINE_SIZE, data);
	i = 0;
	r = 1;
	while (r > 0)
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r <= 0)
			return (safe_free((void **)&buff), NULL);
		buff[i] = c;
		buff[++i] = 0;
		if (c == '\n')
			return (buff);
		if (i > MAX_HERE_LINE_SIZE)
			break ;
	}
	if (i > 0)
		return (buff);
	safe_free((void **)&buff);
	return (NULL);
}

char	*conca_here_doc(char *line, t_parsing_data *token, t_data *data)
{
	int		len;
	char	*conca;
	int		i;
	int		j;

	len = ft_strlen(line) + ft_strlen(token->here);
	conca = safe_malloc(sizeof(char) * (len + 1), data);
	i = 0;
	while (token->here[i])
	{
		conca[i] = token->here[i];
		i++;
	}
	j = 0;
	while (line[j])
		conca[i++] = line[j++];
	conca[i] = '\0';
	safe_free((void **)&token->here);
	return (conca);
}

int	get_here_docs(t_parsing_data *token, t_data *data)
{
	char	*line;

	while (1)
	{
		line = gnl(data);
		if (!line || g_signal[1] == 1)
		{
			safe_free((void **)&token->here);
			token->here = NULL;
			token->here = safe_malloc(sizeof(char), data);
			if (token->here)
				token->here[0] = '\0';
			if (g_signal[1] == 1)
				return (0);
			return (1);
		}
		if (ft_check_line(line, token->here_docs->delimiter))
		{
			safe_free((void **)&line);
			return (1);
		}
		token->here = conca_here_doc(line, token, data);
		safe_free((void **)&line);
	}
}

int	load_here(t_parsing_data *token, t_data *data)
{
	t_here_docs	*tmp;

	while (token)
	{
		while (token->here_docs)
		{
			if (!token->here_docs->delimiter)
				return (ft_putstr_fd("Invalid delimiter in here doc\n", 2), 0);
			safe_free((void **)&token->here);
			token->here = safe_malloc(sizeof(char), data);
			token->here[0] = '\0';
			if (!get_here_docs(token, data))
				return (0);
			tmp = token->here_docs;
			token->here_docs = token->here_docs->next;
			safe_free((void **)&tmp->delimiter);
			safe_free((void **)&tmp);
		}
		token = token->next;
	}
	return (1);
}
