/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:46:43 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:39:20 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	command_not_found_error(char *cmd0)
{
	write(2, cmd0, ft_strlen(cmd0));
	write(2, ": command not found\n", 20);
	exit(127);
}

static char	*resolve_command_path(char *cmd0, t_env *env)
{
	char	*path;

	path = find_command_path(cmd0, env);
	if (!path)
		command_not_found_error(cmd0);
	if (access(path, F_OK) == -1)
	{
		perror(path);
		free(path);
		exit(127);
	}
	if (is_a_directory(path))
	{
		write(2, path, ft_strlen(path));
		write(2, ": Is a directory\n", 17);
		free(path);
		exit(126);
	}
	if (access(path, X_OK) == -1)
	{
		perror(path);
		free(path);
		exit(126);
	}
	return (path);
}

void	child_process(t_command *cmd, t_env *env)
{
	char	*path;

	reset_signals_to_default();
	if (handle_redirections(cmd, env, 0) != SUCCESS)
		exit(EXIT_FAILURE);
	path = resolve_command_path(cmd->argv[0], env);
	if (execve(path, cmd->argv, env_to_array(env)) == -1)
	{
		perror(path);
		free(path);
		exit(126);
	}
}

static int	wait_for_child(pid_t pid)
{
	int					status;
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		return (128 + WTERMSIG(status));
	}
	else
		return (FAILURE);
}

int	execute_external_command(t_command *cmd, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (FAILURE);
	}
	else if (pid == 0)
	{
		child_process(cmd, env);
		exit (EXIT_FAILURE);
	}
	else
		return (wait_for_child(pid));
}
