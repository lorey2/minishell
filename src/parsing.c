/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/01/24 16:23:17 by maambuhl         ###   LAUSANNE.ch       */
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
		|| c == '<' || c == '>' || c == ' ')
		return (1);
	return (0);
}

int	check_meta_char_arg(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>')
		return (1);
	return (0);
}

void	get_value(char **input, t_parsing_data *pars, int make_offset)
{
	int	len;

	len = 0;
	while (ft_isprint((*input)[len]) && !check_meta_char((*input)[len]) && (*input)[len] != ' ')
		len++;
	pars->value = malloc(sizeof(char) * (len + 1));
	if (!pars->value)
		error("malloc error", NULL);
	ft_strlcpy(pars->value, *input, len + 1);
	if (make_offset)
		*input += len;
}

void	get_arg(char **input, t_parsing_data *pars)
{
	int		len;
	char	*arg;

	len = 0;
	while (ft_isprint((*input)[len]) && !check_meta_char_arg((*input)[len]))
		len++;
	if (!len)
		return ;
	arg = malloc(sizeof(char) * (len + 1));
	if (!arg)
		error("malloc error", NULL);
	ft_strlcpy(arg, *input, len + 1);
	pars->arg = ft_split(arg, ' ');
	if (!pars->arg)
		error("malloc error", NULL);
	*input += len;
}

void	handle_in_file(char **input, t_parsing_data *pars)
{
	init_new_token(pars);
	pars->in_file = true;
	++(*input);
	skip_space(input);
	get_value(input, pars, 1);
}

void	handle_out_file(char **input, t_parsing_data *pars)
{
	init_new_token(pars);
	pars->out_file = true;
	++(*input);
	skip_space(input);
	get_value(input, pars, 1);
	skip_space(input);
	if (**input == '|')
	{
		pars->pipe = true;
		++(*input);
	}
}

void	handle_cmd(char **input, t_parsing_data *pars)
{
	init_new_token(pars);
	pars->is_cmd = true;
	skip_space(input);
	get_value(input, pars, 0);
	skip_space(input);
	get_arg(input, pars);
	if (**input == '|')
	{
		pars->pipe = true;
		(*input)++;
	}
}

void	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;
	t_parsing_data	*prev;
	int				pos;

	prev = 0;
	pos = 0;
	while (*input)
	{
		pars = malloc(sizeof(t_parsing_data));
		if (!pars)
			error("malloc error", data);
		if (!prev)
			data->token = pars;
		if (prev)
			prev->next = pars;
		prev = pars;
		skip_space(&input);
		if (*input == '<')
			handle_in_file(&input, pars);
		else if (*input == '>')
			handle_out_file(&input, pars);
		else
			handle_cmd(&input, pars);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
}
