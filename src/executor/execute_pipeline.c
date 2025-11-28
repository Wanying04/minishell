#include "minishell.h"

int	is_valid_pipeline(t_command *cmd)
{
	while (cmd->next && cmd->is_piped == 1)
	{
		if (!cmd || !cmd->argv || !cmd->argv[0])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			return (FAILURE);
		}
		cmd = cmd->next;
	}
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (FAILURE);
	}
	return (SUCCESS);
}

void	execute_pipeline_command(t_command *cmd, t_env *env)
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

void	setup_child_pipe_io(int in_fd, int pipefd[2], t_command *cmd)
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

void	manage_parent_process(int *in_fd, int pipefd[2], t_command *cmd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}	
}

int	wait_for_all(void)
{
	int	status;
	int	last_status;

	last_status = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
	}
	if (last_status == 0)
		return (SUCCESS);
	return (FAILURE);
}

int	execute_pipeline_core(t_command *cmd, t_env *env)
{
	int		in_fd;
	int		pipefd[2];
	pid_t	pid;

	in_fd = STDIN_FILENO;
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
		manage_parent_process(&in_fd, pipefd, cmd);
		cmd = cmd->next;
	}
	return (wait_for_all());
}

int	execute_pipeline(t_command *cmd, t_env *env)
{
	if (is_valid_pipeline(cmd) != SUCCESS)
		return (FAILURE);
	if (execute_pipeline_core(cmd, env) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
