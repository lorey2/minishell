/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:14:27 by lorey             #+#    #+#             */
/*   Updated: 2025/03/29 12:12:22 by lorey            ###   LAUSANNE.ch       */
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
	char	*tmp;
	char	*tmp2;

	tmp2 = ft_substr(data->input,
			pp_data->bkp2, pp_data->i - pp_data->bkp2);
	tmp = ft_strjoin(pp_data->modified, tmp2);
	safe_free((void **)&tmp2);
	safe_free((void **)&pp_data->modified);
	pp_data->modified = tmp;
	home_dir = get_env(data->env, "HOME", data->var);
	if (home_dir)
		tmp = ft_strjoin(pp_data->modified, home_dir);
	else
		tmp = ft_strjoin(pp_data->modified, "/default/home");
	safe_free((void **)&pp_data->modified);
	pp_data->modified = tmp;
	safe_free((void **)&home_dir);
	pp_data->bkp2 = pp_data->i + 1;
}

int	pre_parsing(t_data *data, bool here_doc, t_pre_pars_data *pp_data)
{
	char	*tmp;
	char	*tmp2;

	pp_data->i = -1;
	pp_data->bkp2 = 0;
	pp_data->in_dquotes = false;
	pp_data->modified = NULL;
	while (++(pp_data->i) >= 0)
	{
		if ((data->input)[pp_data->i]
			&& (data->input)[pp_data->i] == '\"' && !here_doc)
			pp_data->in_dquotes = !pp_data->in_dquotes;
		pp_data->i = handle_simple_quote(data->input, pp_data->i, here_doc);
		expansion(data, pp_data);
		if (!((data->input)[pp_data->i]))
		{
			tmp = ft_substr(data->input, pp_data->bkp2, pp_data->i - pp_data->bkp2);
			tmp2 = ft_strjoin(pp_data->modified, tmp);
			safe_free((void **)&pp_data->modified);
			pp_data->modified = tmp2;
			safe_free((void **)&tmp);
			break ;
		}
	}
	if (pp_data->modified[0] != '\0')
	{
		safe_free((void **)&data->input);
		data->input = pp_data->modified;
	}
	printf("input after pre_parsing : [%s]\n", data->input);
	return (0);
}
