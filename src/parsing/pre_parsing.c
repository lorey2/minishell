/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:08:08 by lorey             #+#    #+#             */
/*   Updated: 2025/02/19 14:23:39 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

//TODO mettre ca dans le HEREDOC

#include "minishell.h"

static int	handle_simple_quote(char *input, int i, bool here_doc)
{
	if ((input)[i] && (input)[i] == '\'' && here_doc == false)
	{
		i++;
		while ((input)[i] && (input)[i] != '\'')
			i++;
		if ((input)[i] == '\0')
			write(1, "simple quote [\'] are not closed. \
Undefined (no variable extention)", 66);
	}
	return (i);
}

static int	expansion(
	char **modified, int *bkp2, t_data *data, int i)
{
	char	*var;
	char	*expanded_var;
	int		backup;

	if ((data->input)[i] && (data->input)[i] == '$')
	{
		*modified
			= ft_strjoin(*modified, ft_substr(data->input, *bkp2, i - *bkp2));
		i++;
		backup = i;
		while ((data->input)[i] && (data->input)[i] != ' '
			&& (data->input)[i] != '$'
			&& (data->input)[i] != '\''
			&& (data->input)[i] != '\"')
			i++;
		var = ft_substr(data->input, backup, i - backup - 1);
		expanded_var = get_env(data->env, var);
		*modified = ft_strjoin(*modified, expanded_var);
		*bkp2 = i;
		i--;
	}
	return (i);
}

int	pre_parsing(t_data *data, bool here_doc)
{
	int		i;
	int		bkp2;
	char	*modified;

	i = -1;
	bkp2 = 0;
	modified = ft_strdup("");
	while (++i >= 0)
	{
		i = handle_simple_quote(data->input, i, here_doc);
		i = expansion(&modified, &bkp2, data, i);
		if (!((data->input)[i]))
		{
			modified
				= ft_strjoin(modified, ft_substr(data->input, bkp2, i - bkp2));
			break ;
		}
	}
	if (modified[0] != '\0')
	{
		free(data->input);
		data->input = strdup(modified);
	}
	return (free(modified), 0);
}
