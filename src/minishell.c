/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:46:13 by lorey             #+#    #+#             */
/*   Updated: 2025/01/13 23:16:35 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

void	process(t_path_data *data, char *input)
{
	pid_t	child_pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error("pipe_error", data);
	child_pid = fork();
	if (child_pid == -1)
		error("fork_error", data);
	if (child_pid == 0)
	{
		close(fd[0]);
		close(fd[1]);
		execute(input, data);
	}
	else
	{
		wait(NULL);
		close(fd[1]);
		close(fd[0]);
	}
}

int	main(void)
{
	char		*input;
	char		shell_prompt[1024];
	int			i;
	char		*path;
	t_path_data	path_data;

	path = getenv("PATH");
	path_data.path_split = ft_split(path, ':');
	while (1)
	{
		if (getcwd(shell_prompt, sizeof(shell_prompt)) == NULL)
		{
			perror("getcwd");
			break ;
		}
		i = -1;
		while (shell_prompt[++i])
			;
		shell_prompt[i] = '$';
		shell_prompt[i + 1] = ' ';
		shell_prompt[i + 2] = '\0';
		input = readline(shell_prompt);
		if (!input)
			break ;
		add_history(input);
		process(&path_data, input);
		free(input);
	}
	return (0);
}
