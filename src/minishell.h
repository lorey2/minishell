/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:16:46 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 15:23:10 by maambuhl         ###   LAUSANNE.ch       */
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
# include <sys/types.h>
# include <unistd.h>

extern int		g_signal;

typedef struct s_path_data
{
	char	*path;
	char	**path_split;
	char	**path_split_slash;
	char	*path_with_com;
	char	*oldpwd;
	char	*pwd;
	char	*home;
	bool	is_big_l;
	bool	is_big_p;
	bool	is_at;
	bool	is_e;
}				t_path_data;

typedef struct s_parsing_data
{
	int						fd_in;
	int						fd_out;
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
void		process(t_data *data);
//signal
void		setup_signal(void);
//execution
void		execute(t_data *data, t_parsing_data *token);
//buitins
void		fill(char *data, t_path_data *path_data);
bool		does_contain_only(char *data, char *list_args);
bool		check_builtin(t_data *data, t_parsing_data *p_data);
int			cd(t_parsing_data *p_data, t_path_data *path_data);
void		pwd(t_parsing_data *p_data);
void		echo(t_parsing_data *p_data);
void		unset(t_data *data, t_parsing_data *p_data);
void		env(t_env_data *e_data);
void		init_flags(t_path_data *path_data);
void		write_err(char *message);
//error and free
void		error(char *message, t_data *data);
void		free_double_point(char **point);
void		free_everything(t_data *data);

#endif
