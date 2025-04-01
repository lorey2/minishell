/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_pars_expans.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:06:05 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 01:33:17 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expansion_dollar_only(t_data *data, t_pre_pars_data *pp_data)
{
	char	*temp;
	char	*substr;

	substr = ft_substr(data->input, pp_data->bkp2, pp_data->i - pp_data->bkp2);
	temp = ft_strjoin(pp_data->modified, substr);
	safe_free((void **)&substr);
	safe_free((void **)&pp_data->modified);
	if (data->input[pp_data->i + 1] == '\"'
		|| data->input[pp_data->i + 1] == '\'')
	{
		pp_data->modified = ft_strdup(temp);
		pp_data->bkp2 = pp_data->i + 1;
	}
	else
	{
		pp_data->modified = ft_strjoin(temp, "$");
		pp_data->bkp2 = pp_data->i + 1;
	}
	safe_free((void **)&temp);
}

static void	process_expanded_variable(t_data *data,
			t_pre_pars_data *pp_data, char *var)
{
	char	*expanded_var;
	char	*temp;

	if (var[0] == '?' && var[1] == '\0')
		expanded_var = ft_itoa(data->last_exit);
	else
		expanded_var = get_env(data->env, var, data->var);
	temp = ft_strjoin("\"", expanded_var);
	safe_free((void **)&expanded_var);
	expanded_var = ft_strjoin(temp, "\"");
	safe_free((void **)&temp);
	temp = ft_strjoin(pp_data->modified, expanded_var);
	safe_free((void **)&expanded_var);
	safe_free((void **)&pp_data->modified);
	pp_data->modified = temp;
	safe_free((void **)&var);
}

static void	expansion_variable(t_data *data, t_pre_pars_data *pp_data)
{
	char	*temp;
	char	*temp2;
	char	*var;
	int		backup;

	temp = ft_substr(data->input,
			pp_data->bkp2, pp_data->i - pp_data->bkp2);
	temp2 = ft_strjoin(pp_data->modified, temp);
	safe_free((void **)&pp_data->modified);
	pp_data->modified = temp2;
	safe_free((void **)&temp);
	(pp_data->i)++;
	backup = pp_data->i;
	skip_spe_char(data, pp_data);
	var = ft_substr(data->input, backup, pp_data->i - backup);
	process_expanded_variable(data, pp_data, var);
	pp_data->bkp2 = pp_data->i;
	(pp_data->i)--;
}

void	expansion(t_data *data, t_pre_pars_data *pp_data)
{
	if ((data->input)[pp_data->i] && (data->input)[pp_data->i] == '$')
	{
		if ((data->input)[pp_data->i + 1] == '\0' ||
				(data->input)[pp_data->i + 1] == ' ' ||
				(data->input)[pp_data->i + 1] == '\t' ||
				(data->input)[pp_data->i + 1] == '\'' ||
				(data->input)[pp_data->i + 1] == '\"')
		{
			expansion_dollar_only(data, pp_data);
			return ;
		}
		expansion_variable(data, pp_data);
	}
	else if ((data->input)[pp_data->i] == '~' && !pp_data->in_dquotes &&
				data->input[pp_data->i + 1] != '\'' &&
				data->input[pp_data->i + 1] != '\"')
		expand_tilde(data, pp_data);
}
