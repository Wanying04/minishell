#include "minishell.h"

int	execute(char **args, t_env *env)
{
	pid_t	pid;
	int		status;

	if (is_builtin(args[0]))
		return (execute_builtins(args, env));
	pid = fork();
	if (pid == 0)
	{
		execve(path, args, env);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (-1);
}

int	is_builtin()
