#include "minishell.h"

void	child_process(t_command *cmd, t_env *env)
{
	char	*path;

	reset_signals_to_default();
	if (handle_redirections(cmd, env) != SUCCESS)
		exit(EXIT_FAILURE);
	path = find_command_path(cmd->argv[0], env);
	if (!path)
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		exit(127);
	}
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
	if (execve(path, cmd->argv, env_to_array(env)) == -1)
	{
		perror(path);
		free(path);
		exit(126);
	}
}

static int	wait_for_child(pid_t pid)
{
	int	status;
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
		{
			write(1, "\n", 1);
		}
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
