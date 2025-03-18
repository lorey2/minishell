/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:14:27 by lorey             #+#    #+#             */
/*   Updated: 2025/03/17 22:42:05 by lorey            ###   LAUSANNE.ch       */
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

void	expand_tilde(t_data *data, t_pre_pars_data *pp_data)
{
	char	*home_dir;

	pp_data->modified = ft_strjoin(pp_data->modified, ft_substr(data->input,
				pp_data->bkp2, pp_data->i - pp_data->bkp2));
	home_dir = get_env(data->env, "HOME", data->var);
	if (home_dir)
		pp_data->modified = ft_strjoin(pp_data->modified, home_dir);
	else
		pp_data->modified = ft_strjoin(pp_data->modified, "/default/home");
	pp_data->bkp2 = pp_data->i + 1;
}

int	pre_parsing(t_data *data, bool here_doc, t_pre_pars_data *pp_data)
{
	pp_data->i = -1;
	pp_data->bkp2 = 0;
	pp_data->in_dquotes = false;
	pp_data->modified = ft_strdup("");
	while (++(pp_data->i) >= 0)
	{
		if ((data->input)[pp_data->i]
			&& (data->input)[pp_data->i] == '\"' && !here_doc)
			pp_data->in_dquotes = !pp_data->in_dquotes;
		pp_data->i = handle_simple_quote(data->input, pp_data->i, here_doc);
		expansion(data, pp_data);
		if (!((data->input)[pp_data->i]))
		{
			pp_data->modified
				= ft_strjoin(pp_data->modified, ft_substr(data->input,
						pp_data->bkp2, pp_data->i - pp_data->bkp2));
			break ;
		}
	}
	if (pp_data->modified[0] != '\0')
	{
		free(data->input);
		data->input = strdup(pp_data->modified);
	}
	return (free(pp_data->modified), 0);
}
