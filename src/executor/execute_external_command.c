#include "minishell.h"

void	child_process(t_command *cmd, t_env *env)
{
	char	*path;

	reset_signals_to_default();
	if (handle_redirections(cmd, env) != SUCCESS)
		exit (EXIT_FAILURE);
	path = find_command_path(cmd->argv[0], env);
	if (!path)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
	if (execve(path, cmd->argv, env_to_array(env)) == -1)
	{
		perror(cmd->argv[0]);
		free(path);
		exit(126);
	}
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
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
