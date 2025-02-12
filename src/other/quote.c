/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:23:45 by lorey             #+#    #+#             */
/*   Updated: 2025/02/04 18:00:34 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//to_do FREE EVERYTHING IF MALLOC FAILS
void	*safe_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr == NULL)
	{
		printf("malloc error");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

//here we expand variables
char	*simple_quote(char *arg)
{
	int		i;
	int		j;
	char	*no_left_squote;
	char	*no_squote;

	i = strlen(arg);
	j = 0;
	no_left_squote = safe_malloc(sizeof(char) * (i - 1));
	while (++j < i)
		no_left_squote[j - 1] = arg[j];
	no_left_squote[j - 1] = '\0';
	free(arg);
	if (no_left_squote[i - 2] != '\"')
		return (printf("WARNING there is no [\'] in \
[%s] (we just delete the left one)\n", arg), no_left_squote);
	else
	{
		j = -1;
		no_squote = safe_malloc(sizeof(char) * (i - 2));
		while (++j < i - 2)
			no_squote[j] = no_left_squote[j];
		no_squote[j] = '\0';
		free(no_left_squote);
		return (no_squote);
	}
}

//here we dont
char	*double_quote(char *arg)
{
	int		i;
	int		j;
	char	*no_left_dquote;
	char	*no_dquote;

	i = strlen(arg);
	j = 0;
	no_left_dquote = safe_malloc(sizeof(char) * (i - 1));
	while (++j < i)
		no_left_dquote[j - 1] = arg[j];
	no_left_dquote[j - 1] = '\0';
	free(arg);
	if (no_left_dquote[i - 2] != '\"')
		return (printf("WARNING there is no [\"] in \
[%s] (we just delete the left one)\n", arg), no_left_dquote);
	else
	{
		j = -1;
		no_dquote = safe_malloc(sizeof(char) * (i - 2));
		while (++j < i - 2)
			no_dquote[j] = no_left_dquote[j];
		no_dquote[j] = '\0';
		free(no_left_dquote);
		return (no_dquote);
	}
}

char	*check_for_quote(char *arg)
{
	if (arg[0] == '\'')
		return (simple_quote(arg));
	else if (arg[0] == '\"')
		return (double_quote(arg));
	return (arg);
}
