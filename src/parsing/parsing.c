/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:22:10 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 14:15:56 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//ok

int	handle_cmd(char **input, t_parsing_data *pars, t_data *data)
{
	(*input) += check_vars_count(input);
	pars->is_cmd = true;
	skip_space(input);
	pars->value = get_value(input, data);
	skip_space(input);
	while (check_for_file(input, pars, data))
		;
	get_arg(input, pars, data);
	while (check_for_file(input, pars, data))
		;
	skip_space(input);
	if (**input == '|')
	{
		(*input)++;
		skip_space(input);
		if ((**input == '|' || **input == '\0') && !pars->here_docs)
			return (ft_putstr_fd("syntax error near unexpected token `|'\n"
					, STDERR_FILENO), 0);
		pars->pipe = true;
	}
	return (1);
}

//ok

t_var	*get_last(t_var *var)
{
	t_var	*head;

	head = var;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

int	process_files_and_cmd(char **input, t_parsing_data *pars, t_data *data)
{
	while (check_for_file(input, pars, data))
		;
	if (!handle_cmd(input, pars, data))
		return (0);
	return (1);
}

int	process_variables(char **input, t_data *data)
{
	int	remove_offset;

	while (check_vars_count(input) > 0)
		add_var(input, data);
	if (**input == '\0')
	{
		data->token = NULL;
		return (0);
	}
	remove_offset = check_vars_count(input);
	*input += (remove_offset * -1);
	return (1);
}

int	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;
	t_parsing_data	*prev;
	int				pos;

	prev = NULL;
	pos = 0;
	while (*input)
	{
		if (!process_variables(&input, data))
			return (1);
		pars = safe_malloc(sizeof(t_parsing_data), data);
		pars->previous = prev;
		init_new_token(pars);
		if (!prev)
			data->token = pars;
		if (prev)
			prev->next = pars;
		prev = pars;
		if (!process_files_and_cmd(&input, pars, data))
			return (0);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
	return (1);
}
