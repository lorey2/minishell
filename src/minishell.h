/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:16:46 by lorey             #+#    #+#             */
/*   Updated: 2025/01/16 17:17:26 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdbool.h>

extern int		g_signal;

typedef struct s_path_data
{
	char	*path;
	char	**path_split;
	char	*path_with_com;
	char	**path_with_com_split;
	char	**cmd_split;
}				t_path_data;

typedef struct s_parsing_data
{
	char					*value;
	char					**arg;
	int						pos;
	bool					in_file;
	bool					out_file;
	bool					is_cmd;
	bool					is_after_pipe;
	bool					pipe;
	struct s_parsing_data	*next;
	struct s_parsing_data	*previous;
}							t_parsing_data;

typedef struct s_data
{
	t_parsing_data	*token;
	t_path_data		*path;
}					t_data;

char		**ft_split(const char *s, char c);
size_t		ft_strlen(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
void		error(char *message, t_data *data);
void		free_double_point(char **point);
void		execute(char *command, t_path_data *data);
void		setup_signal(void);
void		parsing(char *input, t_data *data);
void		init_new_token(t_parsing_data *token);

#endif
