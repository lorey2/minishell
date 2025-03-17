/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:09:58 by lorey             #+#    #+#             */
/*   Updated: 2025/03/17 15:38:07 by lorey            ###   LAUSANNE.ch       */
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

static void	expand_tilde(t_data *data, t_pre_pars_data *pp_data)
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

static void	skip_spe_char(t_data *data, t_pre_pars_data *pp_data)
{
	while ((data->input)[pp_data->i] && (data->input)[pp_data->i] != ' '
		&& (data->input)[pp_data->i] != '$'
		&& (data->input)[pp_data->i] != '\''
		&& (data->input)[pp_data->i] != '\"')
		(pp_data->i)++;
}

static void	expansion(t_data *data, t_pre_pars_data *pp_data)
{
	char	*var;
	char	*expanded_var;
	int		backup;

	if ((data->input)[pp_data->i] && (data->input)[pp_data->i] == '$')
	{
		pp_data->modified
			= ft_strjoin(pp_data->modified, ft_substr(data->input,
					pp_data->bkp2, pp_data->i - pp_data->bkp2));
		(pp_data->i)++;
		backup = pp_data->i;
		skip_spe_char(data, pp_data);
		var = ft_substr(data->input, backup, pp_data->i - backup);
		if (var[0] == '?' && var[1] == '\0')
			expanded_var = ft_itoa(data->last_exit);
		else
			expanded_var = get_env(data->env, var, data->var);
		pp_data->modified = ft_strjoin(pp_data->modified, expanded_var);
		free(var);
		pp_data->bkp2 = pp_data->i;
		(pp_data->i)--;
	}
	else if ((data->input)[pp_data->i] == '~' && !pp_data->in_dquotes)
		expand_tilde(data, pp_data);
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
