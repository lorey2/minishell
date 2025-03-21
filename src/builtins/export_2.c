/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:35 by lorey             #+#    #+#             */
/*   Updated: 2025/02/21 15:48:20 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	calculate_rows_and_cols(char **src, int *rows, int *cols)
{
	int	i;
	int	current_len;

	i = 0;
	*cols = 0;
	while (src[i] != NULL)
	{
		(*rows)++;
		current_len = strlen(src[i]);
		if (current_len > *cols)
			*cols = current_len;
		i++;
	}
}

static void	bubble_sort(char **arr, int rows, int max_len)
{
	int		i;
	int		j;
	int		swapped;
	char	*temp;

	i = -1;
	while (++i < rows - 1)
	{
		j = -1;
		swapped = 0;
		while (++j < rows - 1 - i)
		{
			if (strncmp(arr[j], arr[j + 1], max_len) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				swapped = 1;
			}
		}
		if (!swapped)
			break ;
	}
}

static char	**copy_array(char **src, int rows)
{
	int		i;
	char	**dest;

	dest = malloc(rows * sizeof(char *));
	if (!dest)
	{
		perror("Unable to allocate memory for destination array");
		return (NULL);
	}
	i = -1;
	while (++i < rows)
	{
		dest[i] = strdup(src[i]);
		if (!dest[i])
		{
			perror("Unable to allocate memory for destination row");
			while (--i >= 0)
				free(dest[i]);
			free(dest);
			return (NULL);
		}
	}
	return (dest);
}

static void	write_result(t_parsing_data *p_data, int rows, char **dest)
{
	int	i;
	int	j;

	i = -1;
	while (++i < rows)
	{
		write(p_data->fd_out, "declare -x ", 11);
		if (ft_strchr(dest[i], '='))
		{
			j = -1;
			while (dest[i][++j] != '=')
				write(p_data->fd_out, &dest[i][j], 1);
			write(p_data->fd_out, "=\"", 2);
			while (dest[i][++j])
				write(p_data->fd_out, &dest[i][j], 1);
			write(p_data->fd_out, "\"\n", 2);
		}
		else
		{
			write(p_data->fd_out, dest[i], ft_strlen(dest[i]));
			write(p_data->fd_out, "\n", 1);
		}
	}
}

void	copy_and_sort_array(char **src, t_parsing_data *p_data)
{
	int		rows;
	int		cols;
	int		i;
	char	**dest;

	rows = 0;
	cols = 0;
	calculate_rows_and_cols(src, &rows, &cols);
	dest = copy_array(src, rows);
	if (dest == NULL)
		perror("Unable to allocate memory for destination array");
	bubble_sort(dest, rows, cols);
	write_result(p_data, rows, dest);
	i = -1;
	while (++i < rows)
		free(dest[i]);
	free(dest);
}
