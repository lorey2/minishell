/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/02/20 16:17:28 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}

void	handle_out_file(char **input, t_parsing_data *pars)
{
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

int	check_vars_count(char **input)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	found = 0;
	skip_space(input);
	while ((*input)[i])
	{
		j = 0;
		while (ft_isalpha((*input)[i + j]) || (*input)[i + j] == '_')
			j++;
		if (((*input)[i + j] != '=' || !j) && !found)
			return (0);
		else if (((*input)[i + j] != '=') && found)
			return (i * -1);
		++i;
		i += j;
		while ((*input)[i] != ' ' && (*input)[i] != '\t' && (*input)[i])
			i++;
		while ((*input)[i] == ' ' || (*input)[i] == '\t')
			i++;
		found = 1;
	}
	return (i);
}

void	handle_cmd(char **input, t_parsing_data *pars)
{
	(*input) += check_vars_count(input);
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

t_var	*allocate_var(char **input)
{
	t_var	*var;
	int		name_len;
	int		value_len;

	var = malloc(sizeof(t_var));
	init_new_var(var);
	if (!var)
		error("malloc error", NULL);
	name_len = 0;
	while ((*input)[name_len] != '=')
		name_len++;
	var->var_name = malloc(sizeof(name_len + 1));
	if (!var->var_name)
		error("malloc error", NULL);
	++name_len;
	value_len = 0;
	while (((*input)[name_len] != ' ' || (*input)[name_len] != '\t') && (*input)[name_len] != '\0')
	{
		name_len++;
		value_len++;
	}
	var->var_value = malloc(sizeof(value_len + 1));
	if (!var->var_value)
		error("malloc error", NULL);
	return (var);
}

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

void	add_var(char **input, t_data *data)
{
	t_var	*var;
	t_var	*last_var;
	int		i;

	i = 0;
	skip_space(input);
	var = allocate_var(input);
	while (**input != '=')
	{
		var->var_name[i++] = **input;
		(*input)++;
	}
	(*input)++;
	var->var_name[i] = '\0';
	i = 0;
	while ((**input != ' ' && **input != '\t') && **input)
	{
		var->var_value[i++] = **input;
		(*input)++;
	}
	var->var_value[i] = '\0';
	if (!data->var)
		data->var = var;
	else
	{
		last_var = get_last(data->var);
		last_var->next = var;
	}
}

void	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;
	t_parsing_data	*prev;
	int				pos;
	int				remove_offset;

	prev = NULL;
	pos = 0;
	while (*input)
	{
		while (check_vars_count(&input) > 0)
			add_var(&input, data);
		if (*input == '\0')
		{
			data->token = NULL;
			return ;
		}
		remove_offset = check_vars_count(&input);
		input += (remove_offset * -1);
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
		while (check_for_file(&input, pars))
			;
		handle_cmd(&input, pars);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
}

