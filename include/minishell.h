/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 21:16:46 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 15:44:07 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/include/libft.h"
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
# include <termios.h>

# define MAX_HERE_LINE_SIZE 1000

# define RESET        "\033[0m"
# define BLACK        "\033[0;30m"
# define RED          "\033[0;31m"
# define GREEN        "\033[0;32m"
# define YELLOW       "\033[0;33m"
# define BLUE         "\033[0;34m"
# define MAGENTA      "\033[0;35m"
# define CYAN         "\033[0;36m"
# define WHITE        "\033[0;37m"
# define BRIGHT_RED   "\033[0;91m"
# define BRIGHT_GREEN "\033[0;92m"

extern volatile sig_atomic_t	g_signal[2];

typedef struct s_path_data
{
	char	*env_path;
	char	**path_split;
	char	**path_split_slash;
	char	*path_with_com;
	bool	is_big_l;
	bool	is_big_p;
	bool	is_big_e;
	bool	is_at;
	bool	is_e;
	bool	is_n;
	bool	is_v;
	bool	is_f;
	bool	is_p;
}				t_path_data;

typedef struct s_term
{
	struct termios	original;
	struct termios	modified;
}					t_term;

typedef struct s_file
{
	char			*name;
	bool			append;
	struct s_file	*next;
}					t_file;

typedef struct s_here_docs
{
	char				*delimiter;
	struct s_here_docs	*next;
}						t_here_docs;

typedef struct s_parsing_data
{
	pid_t					pid;
	int						fd_in;
	int						fd_out;
	char					*value;
	char					*delimiter;
	char					**arg;
	char					*here;
	char					*infile;
	char					*outfile;
	int						pos;
	int						saved_stdin;
	int						status;
	bool					in_file;
	bool					out_file;
	bool					append_file;
	bool					is_cmd;
	bool					is_after_pipe;
	bool					pipe;
	t_file					*outfile_list;
	t_file					*infile_list;
	t_here_docs				*here_docs;
	struct s_parsing_data	*next;
	struct s_parsing_data	*previous;
}							t_parsing_data;

typedef struct s_pre_parsing_data
{
	int		i;
	bool	in_dquotes;
	char	*modified;
	int		bkp2;
}			t_pre_pars_data;

typedef struct s_env_data
{
	char	**env;
}			t_env_data;

typedef struct s_var
{
	char			*var_name;
	char			*var_value;
	struct s_var	*next;
}					t_var;

typedef struct s_data
{
	t_parsing_data	*token;
	t_path_data		*path;
	t_env_data		*env;
	t_var			*var;
	t_pre_pars_data	*pp_data;
	char			*input;
	int				exit_nbr;
	int				last_exit;
	int				return_nbr;
}					t_data;

//setup_shell
void			setup_terminal(t_term *term);
void			restore_terminal(t_term *term);
//animation
void			text_animation(void);
void			explosion_animation(void);
//setup
void			setup_env(t_data *data, char **env);
void			setup_path(t_data *data, t_path_data *path_data);
char			*setup_prompt(t_data *data);
void			init_path(t_path_data *path_data);
void			init_struct(t_data *data);
// pre_parsing
int				pre_parsing(t_data *data, bool here_doc,
					t_pre_pars_data *pp_data);
void			expansion(t_data *data, t_pre_pars_data *pp_data);
void			expand_tilde(t_data *data, t_pre_pars_data *pp_data);
void			skip_spe_char(t_data *data, t_pre_pars_data *pp_data);
//parsing
int				parsing(char *input, t_data *data);
void			setup_token(char **input, t_data *data,
					t_parsing_data **pars);
t_var			*get_last(t_var *var);
int				handle_cmd(char **input, t_parsing_data *pars, t_data *data);
char			*extract_unquoted_value(char **input, t_data *data);
char			*extract_quoted_value(char **input, t_data *data);
int				get_value(char **input, char **to_set,
					t_data *data);
void			init_new_here(t_here_docs *here);
void			init_new_token(t_parsing_data *token);
void			init_new_var(t_var *var);
void			init_new_file(t_file *file);
void			process(t_data *data);
	//helpers
int				is_quote(char c);
void			skip_space(char **input);
int				check_meta_char(char c);
int				check_meta_char_arg(char c);
	//here_doc.c
char			*check_here_doc_del(char **input, t_data *data);
t_here_docs		*get_last_here(t_here_docs *here);
void			handle_here(char **input, t_parsing_data *pars, t_data *data);
	//var
int				check_vars_count(char **input);
t_var			*allocate_var(char **input, t_data *data);
void			add_var(char **input, t_data *data);
	//files
t_file			*get_last_file(t_file *file);
void			allocate_file(char **input, t_parsing_data *pars,
					t_data *data);
void			handle_in_file(char **input, t_parsing_data *pars,
					t_data *data);
void			handle_out_file(char **input, t_parsing_data *pars,
					t_data *data);
int				check_for_file(char **input, t_parsing_data *pars,
					t_data *data);
	//arg
void			rebuild_arg(t_parsing_data *pars, t_data *data);
int				get_length(char **input);
int				set_reset_quote(int *in_quote, char *quote_char,
					int i, char **input);
void			get_arg(char **input, t_parsing_data *pars, t_data *data);
char			*extract_content(char **input, int len, t_data *data);

//signal
void			setup_signal(void);
//execution
void			execute(t_data *data, t_parsing_data *token);
void			wait_for_all(t_data *data);
void			final_wait(t_data *data);
//buitins
void			frexit(t_data *data, int exit_number);
bool			exec_builtin(t_data *data, t_parsing_data *p_data);
bool			is_builtin(char *cmd);
void			fill(char *data, t_path_data *path_data);
bool			does_contain_only(char *data, char *list_args);
bool			check_builtin(t_data *data, t_parsing_data *p_data);
int				cd(t_parsing_data *p_data, t_path_data *path_data,
					t_data *data);
int				do_cd_update_env(char *arg, t_data *data);
void			pwd(t_parsing_data *p_data, t_path_data *path_data,
					t_data *data);
void			echo(t_parsing_data *p_data, t_path_data *path_data,
					t_data *data);
int				unset(t_env_data *e_data,
					t_path_data *path_data, t_parsing_data *p_data);
void			env(t_env_data *e_data, t_parsing_data *p_data, t_data *data);
void			init_flags(t_path_data *path_data);
void			write_err(char *message);
int				mini_exit(t_data *data, t_parsing_data *p_data);
int				mini_export(t_parsing_data *p_data, t_path_data *path_data,
					t_var *v_data, t_data *data);
int				e_setup_flags(t_parsing_data *p_data, t_path_data *path_data);
void			copy_and_sort_array(char **src, t_parsing_data *p_data,
					t_data *data);
//update env
void			set_env(char *var_name, char *value, bool is_equal,
					t_data *data);
char			*get_env(t_env_data *e_data, char *var_name, t_var *var);
void			write_env_error(char *var_name, char *message);
bool			is_valid_var_name(char	*arg);
//error and free
void			error(char *message, t_data *data);
void			free_double_point(char ***point);
void			free_everything(t_data *data);
void			free_path(t_path_data *path_data);
void			free_tokens(t_parsing_data *token, t_data *data);
//gnl
char			*get_next_line(int fd);
//utils
void			*safe_malloc(size_t size, t_data *data);
void			safe_free(void **ptr);
//here_doc
char			*gnl(t_data *data);
char			*conca_here_doc(char *line, t_parsing_data *token,
					t_data *data);
int				get_here_docs(t_parsing_data *token, t_data *data);
int				load_here(t_parsing_data *token, t_data *data);
// exec
int				count_pipe(t_data *data);
int				open_in_file(char *file_name, t_data *data);
int				open_file(t_file *file, t_data *data);
t_parsing_data	*get_last_token(t_parsing_data *token);
void			free_heres(t_parsing_data *token);
void			execute_command_pipe(t_data *data, t_parsing_data *token,
					int pipefd[2]);
void			execute(t_data *data, t_parsing_data *token);
void			pipex(t_data *data, t_parsing_data *token);
int				check_out_file(t_parsing_data *token, t_data *data);
int				check_in_file(t_parsing_data *token, t_data *data);
int				check_file(t_parsing_data *token, t_data *data,
					int saved_stdin);
void			here_doc_write(t_parsing_data *token, int *pipefd);
void			here_doc(t_parsing_data *token, t_data *data);

#endif
