#include "minishell.h"

static void	setup_child_pipe_io(int in_fd, int pipefd[2], t_command *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	if (cmd->next)
		close(pipefd[0]);
}

static void	execute_pipeline_command(t_command *cmd, t_env *env)
{
	int	status;

	if (is_builtin_command(cmd) == SUCCESS)
	{
		status = execute_builtins(cmd, env);
		exit(status);
	}
	else
		child_process(cmd, env);
}

static void	manage_parent_process(int *in_fd, int pipefd[2], t_command *cmd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}	
}

static int	wait_for_all(pid_t last_pid)
{
	int		status;
	int		last_status;
	pid_t	pid;

	last_status = SUCCESS;
	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	return (last_status);
}

int	execute_pipeline_core(t_command *cmd, t_env *env)
{
	int		in_fd;
	int		pipefd[2];
	pid_t	pid;
	pid_t	last_pid;

	in_fd = STDIN_FILENO;
	last_pid = 0;
	while (cmd)
	{
		if (cmd->next && pipe(pipefd) == -1)
			return (perror("pipe"), FAILURE);
		if ((pid = fork()) == -1)
			return (perror("fork"), FAILURE);
		if (pid == 0)
		{
			setup_child_pipe_io(in_fd, pipefd, cmd);
			execute_pipeline_command(cmd, env);
			exit (EXIT_FAILURE);
		}
		last_pid = pid;
		manage_parent_process(&in_fd, pipefd, cmd);
		cmd = cmd->next;
	}
	return (wait_for_all(last_pid));
}
