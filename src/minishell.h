/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:16:46 by lorey             #+#    #+#             */
/*   Updated: 2025/01/13 22:01:30 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_data
{
	char	*path;
	char	**path_split;
	char	*path_with_com;
	char	**path_with_com_split;
	char	**cmd_split;
	int		nbr_malloc;
	int		input_file;
	int		output_file;
	int		process;
	char	*line;
	int		fd[2];
	pid_t	here_pid;
}				t_path_data;

char		**ft_split(const char *s, char c);
size_t		ft_strlen(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
void		error(char *message, t_path_data *data);
void		free_double_point(char **point);
void		execute(char *command, t_path_data *data);

#endif
