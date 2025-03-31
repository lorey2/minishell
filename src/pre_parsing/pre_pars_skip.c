/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_pars_skip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:56:34 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 13:56:57 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spe_char(t_data *data, t_pre_pars_data *pp_data)
{
	while ((data->input)[pp_data->i] && (data->input)[pp_data->i] != ' '
		&& (data->input)[pp_data->i] != '$'
		&& (data->input)[pp_data->i] != '\''
		&& (data->input)[pp_data->i] != '\"')
		(pp_data->i)++;
}
