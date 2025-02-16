/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/02/12 16:25:41 by maambuhl         ###   LAUSANNE.ch       */
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

char	*get_value(char **input, t_parsing_data *pars, int make_offset)
{
	int		len;
	char	*value;
	(void)pars;

	len = 0;
	while (ft_isprint((*input)[len]) && !check_meta_char((*input)[len]) && (*input)[len] != ' ')
		len++;
	value = malloc(sizeof(char) * (len + 1));
	if (!value)
		error("malloc error", NULL);
	if (!len)
		return (NULL);
	ft_strlcpy(value, *input, len + 1);
	// if (make_offset)
	(void)make_offset;
		*input += len;
	return (value);
}

void	rebuild_arg(t_parsing_data *pars)
{
	int		i;
	int		j;
	char	**arg_with_cmd;

	i = 0;
	j = 0;
	while (pars->arg[i])
		i++;
	arg_with_cmd = malloc(sizeof(char *) * i + 2);
	arg_with_cmd[j++] = ft_strdup(pars->value);
	i = 0;
	while (pars->arg[i])
		arg_with_cmd[j++] = ft_strdup(pars->arg[i++]);
	arg_with_cmd[j] = NULL;
	free_double_point(pars->arg);
	pars->arg = arg_with_cmd;
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
	rebuild_arg(pars);
}

char	*check_here_doc_del(char **input)
{
	int		len;
	char	*del;

	skip_space(input);
	len = 0;
	// check here doc special delimiter cases: '', "", | etc
	while ((*input)[len] != ' ' && (*input)[len] != ';' && (*input)[len])
		len++;
	del = malloc(sizeof(char) * (len + 1));
	if (!del)
		error("malloc error", NULL);
	len = 0;
	while (**input != ' ' && **input != ';' && **input)
	{
		del[len] = **input;
		++(*input);
		len++;
	}
	del[len] = 0;
	return (del);
}

void	handle_in_file(char **input, t_parsing_data *pars)
{
	// init_new_token(pars);
	++(*input);
	if (**input == '<')
	{
		++(*input);
		pars->delimiter = check_here_doc_del(input);
		pars->infile = NULL;
	}
	else
	{
		pars->in_file = true;
		skip_space(input);
		pars->infile = get_value(input, pars, 1);
		pars->delimiter = NULL;
	}
	skip_space(input);
	// if (**input == '|')
	// {
	// 	pars->pipe = true;
	// 	++(*input);
	// }
}

void	handle_out_file(char **input, t_parsing_data *pars)
{
	// init_new_token(pars);
	pars->out_file = true;
	++(*input);
	if (**input == '>')
	{
		pars->append_file = true;
		++(*input);
	}
	skip_space(input);
	pars->outfile = get_value(input, pars, 1);
	skip_space(input);
	// if (**input == '|')
	// {
	// 	pars->pipe = true;
	// 	++(*input);
	// }
}

int	check_for_file(char **input, t_parsing_data *pars)
{
	if (**input == '<')
	{
		handle_in_file(input, pars);
		return (1);
	}
	else if (**input == '>')
	{
		handle_out_file(input, pars);
		return (1);
	}
	return (0);
}

void	handle_cmd(char **input, t_parsing_data *pars)
{
	pars->is_cmd = true;
	skip_space(input);
	pars->value = get_value(input, pars, 0);
	skip_space(input);
	while (check_for_file(input, pars))
		;
	get_arg(input, pars);
	while (check_for_file(input, pars))
		;
	skip_space(input);
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
		pars->previous = prev;
		if (!pars)
			error("malloc error", data);
		if (!prev)
			data->token = pars;
		if (prev)
			prev->next = pars;
		prev = pars;
		init_new_token(pars);
		skip_space(&input);
		// if (*input == '<')
		// 	handle_in_file(&input, pars);
		// else if (*input == '>')
		// 	handle_out_file(&input, pars);
		// else if (*input == '<')
		// 	handle_in_file(&input, pars);
		// else
		while (check_for_file(&input, pars))
			;
		handle_cmd(&input, pars);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
}

