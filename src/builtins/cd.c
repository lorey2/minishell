/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:33:57 by lorey             #+#    #+#             */
/*   Updated: 2025/03/04 21:18:08 by lorey            ###   LAUSANNE.ch       */
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

// here we chdir and we update the vars cwd and oldpwd that are normally in env

static int	do_cd_update_env(char *arg, t_env_data *e_data)
{
	char	cwd[1024];

	if (chdir(arg) == -1)
		return (write_err("cd : No such file or directory\n"), 1);
	set_env(e_data, "OLDPWD", get_env(e_data, "PWD", NULL), true);
	if (get_env(e_data, "OLDPWD", NULL) == NULL)
		return (write_err("cd : Memory allocation failed for OLDPWD\n"), 1);
	getcwd(cwd, 1024);
	set_env(e_data, "PWD", cwd, true);
	if (get_env(e_data, "PWD", NULL) == NULL)
		return (write_err("cd : Memory allocation failed for PWD\n"), 1);
	return (0);
}

// here we only handle cd -
// it does cd to the last directory in which we had cd

static int	only_dash(t_parsing_data *p_data, t_env_data *e_data, int i)
{
	char	*oldpwd;

	oldpwd = get_env(e_data, "OLDPWD", NULL);
	if (ft_isequal(p_data->arg[i], "-"))
	{
		if (oldpwd == NULL)
			return (write_err("cd : OLDPWD not set\n"), 1);
		else if (p_data->arg[i + 1])
			return (write_err("cd : too many arguments\n"), 1);
		write(p_data->fd_out, oldpwd, ft_strlen(oldpwd));
		write(p_data->fd_out, "\n", 1);
		return (do_cd_update_env(oldpwd, e_data), 1);
	}
	return (0);
}

// here we handle -- and false arguments

static int	check_dash(t_parsing_data *p_data, t_env_data *e_data
		, char *home, int i)
{
	if (p_data->arg[i][0] == '-')
	{
		if (only_dash(p_data, e_data, i) == 1)
			return (1);
		else if (ft_isequal(p_data->arg[i], "--"))
		{
			if (!p_data->arg[i + 1])
			{
				if (!home)
					return (write_err(
							"cd : HOME environment variable not set\n"), 1);
				else
					do_cd_update_env(home, e_data);
			}
			else if (p_data->arg[i + 2])
				return (write_err("cd : too many argumets\n"), 1);
			else
				do_cd_update_env(p_data->arg[2], e_data);
		}
		else
			return (write_err("cd : Illegal option (--, -,[working])\n \
                     (-L, -P, -@, -e[not implemented])\n"), 1);
	}
	return (0);
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

//here is the main cd file that handle ~ and no args

int	cd(t_parsing_data *p_data, t_path_data *path_data, t_env_data *e_data)
{
	int	i;

	i = setup_flags(p_data, path_data);
	if (i == -1)
		return (1);
	if (p_data->arg[i])
	{
		if (check_dash(p_data, e_data, get_env(e_data, "HOME", NULL), i))
			return (1);
		else if (p_data->arg[i + 1])
			return (write_err("cd : too many arguments\n"), 1);
		else if (ft_isequal(p_data->arg[i], "~"))
		{
			if (!get_env(e_data, "HOME", NULL))
				return (write_err("cd : HOME environment variable not set\n"), \
				1);
			return (do_cd_update_env(get_env(e_data, "HOME", NULL), e_data), 0);
		}
		return (do_cd_update_env(p_data->arg[i], e_data), 0);
	}
	if (!get_env(e_data, "HOME", NULL))
		return (write_err("cd : HOME environment variable not set\n"), 1);
	return (do_cd_update_env(get_env(e_data, "HOME", NULL), e_data), 0);
}
