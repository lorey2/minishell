/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 18:22:52 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// TODO error gestion
//
void	skip_space(char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}

int	check_meta_char(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>' || c == ' ' || c == 't')
		return (1);
	return (0);
}

void	handle_in_file(char *input, t_parsing_data *pars)
{
	int	len;

	len = 0;
	init_new_token(pars);
	pars->in_file = true;
	while (ft_isalpha(input[len]) && !check_meta_char(input[len]))
		len++;
	pars->value = malloc(sizeof(char) * (len + 1));
	if (!pars->value)
		error("malloc error", NULL);
	ft_strlcpy(pars->value, input, len + 1);
	printf("value = %s\n", pars->value);
	// input += len;
}

void	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;

	pars = malloc(sizeof(t_parsing_data));
	if (!pars)
		error("malloc error", data);
	data->token = pars;
	while (*input)
	{
		skip_space(&input);
		if (*input == '<')
		{
			input++;
			skip_space(&input);
			handle_in_file(input, pars);
		}
		input++;
	}
}






//     ls    -la    >    output.txt |   cat   -e 
