/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_pars_expans.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:37:24 by lorey             #+#    #+#             */
/*   Updated: 2025/03/17 22:42:12 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_spe_char(t_data *data, t_pre_pars_data *pp_data)
{
	while ((data->input)[pp_data->i] && (data->input)[pp_data->i] != ' '
		&& (data->input)[pp_data->i] != '$'
		&& (data->input)[pp_data->i] != '\''
		&& (data->input)[pp_data->i] != '\"')
		(pp_data->i)++;
}

static void
	expansion_quoted(t_data *data, t_pre_pars_data *pp_data, char quote_type)
{
	int		j;
	char	*quoted_content;

	j = pp_data->i + 2;
	pp_data->modified = ft_strjoin(pp_data->modified, ft_substr(data->input,
				pp_data->bkp2, pp_data->i - pp_data->bkp2));
	while ((data->input)[j] && (data->input)[j] != quote_type)
		j++;
	if ((data->input)[j] == quote_type)
	{
		quoted_content
			= ft_substr(data->input, pp_data->i + 2, j - (pp_data->i + 2));
		pp_data->modified = ft_strjoin(pp_data->modified, quoted_content);
		free(quoted_content);
		pp_data->bkp2 = j + 1;
		pp_data->i = j;
	}
	else
	{
		pp_data->modified = ft_strjoin(pp_data->modified, "$");
		pp_data->bkp2 = pp_data->i + 1;
		pp_data->i++;
	}
}

static void	expansion_dollar_only(t_data *data, t_pre_pars_data *pp_data)
{
	pp_data->modified = ft_strjoin(pp_data->modified, ft_substr(data->input,
				pp_data->bkp2, pp_data->i - pp_data->bkp2));
	pp_data->modified = ft_strjoin(pp_data->modified, "$");
	pp_data->bkp2 = pp_data->i + 1;
	(pp_data->i)++;
}

static void	expansion_variable(t_data *data, t_pre_pars_data *pp_data)
{
	int		backup;
	char	*var;
	char	*expanded_var;

	pp_data->modified = ft_strjoin(pp_data->modified, ft_substr(data->input,
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

void	expansion(t_data *data, t_pre_pars_data *pp_data)
{
	if ((data->input)[pp_data->i] && (data->input)[pp_data->i] == '$')
	{
		if ((data->input)[pp_data->i + 1] == '\''
			|| (data->input)[pp_data->i + 1] == '\"')
		{
			expansion_quoted(data, pp_data, (data->input)[pp_data->i + 1]);
			return ;
		}
		else if ((data->input)[pp_data->i + 1] == '\0' ||
				(data->input)[pp_data->i + 1] == ' ' ||
				(data->input)[pp_data->i + 1] == '\t')
		{
			expansion_dollar_only(data, pp_data);
			return ;
		}
		expansion_variable(data, pp_data);
	}
	else if ((data->input)[pp_data->i] == '~' && !pp_data->in_dquotes)
		expand_tilde(data, pp_data);
}
