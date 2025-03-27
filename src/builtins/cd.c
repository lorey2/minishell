/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:33:57 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 22:34:27 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

//cd accept only one arg and a list of flags
//if you give more than one arg the rest is ignored
//the list of flags and interesting things are
//	cd -
//		gives the previous directory witch is stored in env OLDPWD
//	cd ~
//		/home
//	cd -P
//		forces the shell to follow symbolic links to their physical location
//	cd -L
//		forces the shell to follow symbolic links and navigate through them.
//		This is often the default behavior
//	cd -- -dir
//		with this flag we can now take dir starting with -
//
//		WE WONT MAkE -P -L -@ and -e...
//		THE REST WORKS

// here we only handle cd -
// it does cd to the last directory in which we had cd

static int	only_dash(t_parsing_data *p_data, int i, t_data *data)
{
	char	*oldpwd;

	if (ft_isequal(p_data->arg[i], "-"))
	{
		oldpwd = get_env(data->env, "OLDPWD", NULL);
		if (oldpwd == NULL)
			return (safe_free((void **)&oldpwd),
				write_err("cd : OLDPWD not set\n"), -1);
		else if (p_data->arg[i + 1])
			return (safe_free((void **)&oldpwd),
				write_err("cd : too many arguments\n"), -1);
		write(p_data->fd_out, oldpwd, ft_strlen(oldpwd));
		write(p_data->fd_out, "\n", 1);
		return (do_cd_update_env(oldpwd, data),
			safe_free((void **)&oldpwd), 1);
	}
	return (0);
}

// here we handle -- and false arguments

static int	check_dash(t_parsing_data *p_data, char *home, int i, t_data *data)
{
	if (p_data->arg[i][0] == '-')
	{
		if (only_dash(p_data, i, data) == 1)
			return (-3);
		else if (ft_isequal(p_data->arg[i], "--"))
		{
			if (!p_data->arg[i + 1])
			{
				if (!home)
					return (write_err(
							"cd : HOME environment variable not set\n"), -1);
				else
					return (do_cd_update_env(home, data), -3);
			}
			else
				return (write_err("cd : too many arguments\n"), -1);
		}
		else
			return (write_err("cd : Illegal option (--, -,[working])\n \
                     (-L, -P, -@, -e[not implemented])\n"), -2);
	}
	return (i);
}

// here we setup flags if we want to do them one day
// flags can be -P -PPPPPPPP {-PP -PPPPLLLLPLPLP}...
// LPe@

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "LPe@"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_big_l || path_data->is_big_p
		|| path_data->is_e || path_data->is_at)
		return (write_err(" -L, -P, -e and -@ are not implemented yet :'(\n"), \
				-1);
	return (i);
}

static int	cd_with_arg(int *i, t_parsing_data *p_data, t_data *data)
{
	char	*home;

	home = get_env(data->env, "HOME", NULL);
	*i = check_dash(p_data, home, *i, data);
	safe_free((void **)&home);
	if (*i == -1)
		return (p_data->status = 1, 1);
	if (*i == -3)
		return (0);
	if (*i == -2)
		return (p_data->status = 2, 1);
	else if (p_data->arg[(*i) + 1])
		return (p_data->status = 1
			, write_err("cd : too many arguments\n"), 1);
	*i = do_cd_update_env(p_data->arg[*i], data);
	if (*i == -1)
		return (p_data->status = 1, 1);
	return (0);
}

//here is the main cd file that handle ~ and no args

int	cd(t_parsing_data *p_data, t_path_data *path_data, t_data *data)
{
	int		i;
	char	*home;

	home = NULL;
	p_data->status = 0;
	if (p_data->pipe)
		exit(0);
	i = setup_flags(p_data, path_data);
	if (i == -1)
		return (p_data->status = 1, 1);
	if (p_data->arg[i])
	{
		if (cd_with_arg(&i, p_data, data))
			return (1);
		return (0);
	}
	home = get_env(data->env, "HOME", NULL);
	if (!home)
		return (p_data->status = 1
			, write_err("cd : HOME environment variable not set\n"), 1);
	i = do_cd_update_env(home, data);
	safe_free((void **)&home);
	if (i == -1)
		return (p_data->status = 1, 1);
	return (0);
}
