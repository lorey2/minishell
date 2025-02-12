/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:08:08 by lorey             #+#    #+#             */
/*   Updated: 2025/02/12 03:04:50 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//DONC EN GROS
//ON ITERE SUR INPUT TANT QU'ON TROUVE PAS DE $
//

int	pre_parsing(char **input, t_data *data)
{
	int		i;
	int		backup;
	int		bkp2;
	char	*var;
	char	*expanded_var;
	char	*modified;

	i = -1;
	bkp2 = 0;
	modified = ft_strdup("");
	while (++i < (int)ft_strlen(*input) && (*input)[i])
	{
		if ((*input)[i] == '\'')
		{
			while ((*input[i]) && (*input[i]) != '\'')
				i++;
			if ((*input[i]) == '\0')
			{
				write(1, "simple quote [\'] are not closed. \
Undefined (no variable extention)", 66);
				break ;
			}
		}
		if ((*input)[i] == '$')
		{
			modified = ft_strjoin(modified, ft_substr(*input, bkp2, i - bkp2));
			i++;
			backup = i;
			while ((*input)[i] && (*input)[i] != ' ' && (*input)[i] != '$' && (*input)[i] != '\'')
				i++;
			var = ft_substr(*input, backup, i - backup - 1);
			expanded_var = get_env(data->env, var);
			modified = ft_strjoin(modified, expanded_var);
			bkp2 = i;
			i--;
		}
	}
	if (modified[0] != '\0')
	{
		free(*input);
		*input = strdup(modified);
	}
	printf("\n%s\n", *input);
	free(modified);
	return (0);
}
