/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:15:52 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 03:17:36 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*get_last_file(t_file *file)
{
	t_file	*head;

	head = file;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

void	allocate_file(char **input, t_parsing_data *pars, t_data *data)
{
	t_file	*file;
	t_file	*last_file;

	file = safe_malloc(sizeof(t_file), data);
	init_new_file(file);
	get_value(input, &pars->infile, data);
	file->name = pars->infile;
	if (!pars->infile_list)
		pars->infile_list = file;
	else
	{
		last_file = get_last_file(pars->infile_list);
		last_file->next = file;
	}
	skip_space(input);
}

void	handle_in_file(char **input, t_parsing_data *pars, t_data *data)
{
	++(*input);
	pars->in_file = true;
	if (**input == '<')
	{
		pars->infile = NULL;
		++(*input);
		handle_here(input, pars, data);
	}
	else
	{
		skip_space(input);
		allocate_file(input, pars, data);
		pars->delimiter = NULL;
	}
	skip_space(input);
}

void	handle_out_file(char **input, t_parsing_data *pars, t_data *data)
{
	t_file	*file;
	t_file	*last_file;

	file = safe_malloc(sizeof(t_file), data);
	init_new_file(file);
	pars->out_file = true;
	++(*input);
	if (**input == '>')
	{
		pars->append_file = true;
		file->append = true;
		++(*input);
	}
	skip_space(input);
	get_value(input, &pars->outfile, data);
	file->name = pars->outfile;
	if (!pars->outfile_list)
		pars->outfile_list = file;
	else
	{
		last_file = get_last_file(pars->outfile_list);
		last_file->next = file;
	}
	skip_space(input);
}

int	check_for_file(char **input, t_parsing_data *pars, t_data *data)
{
	if (**input == '<')
	{
		handle_in_file(input, pars, data);
		return (1);
	}
	else if (**input == '>')
	{
		handle_out_file(input, pars, data);
		return (1);
	}
	return (0);
}
