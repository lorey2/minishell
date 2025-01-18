/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:16:46 by lorey             #+#    #+#             */
/*   Updated: 2025/01/18 20:05:03 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
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

typedef struct s_env_data
{
	char	**env;
	char	**path;
	char	**home;
	char	**cwd;
}			t_env_data;

typedef struct s_data
{
	t_parsing_data	*token;
	t_path_data		*path;
	t_env_data		*env;
}					t_data;

//setup
void		setup_env(t_data *data, char **env);
void		setup_path(t_path_data *path_data);
char		*setup_prompt(t_data *data);
void		init_struct(t_data *data);
//parsing
void		parsing(char *input, t_data *data);
void		init_new_token(t_parsing_data *token);
//signal
void		setup_signal(void);
//execution
void		execute(char *command, t_path_data *data);
//buitins
void		cd(t_data *data, t_parsing_data *p_data);
void		pwd(t_parsing_data *p_data);
void		echo(t_parsing_data *p_data);
void		unset(t_data *data, t_parsing_data *p_data);
void		env(t_env_data *e_data);
//error and free
void		error(char *message, t_data *data);
void		free_double_point(char **point);
void		free_everything(t_data *data);

#endif
