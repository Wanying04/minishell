#include "minishell.h"

int	is_builtin_command(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0 && cmd->argv[0][4] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "cd", 2) == 0 && cmd->argv[0][2] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0 && cmd->argv[0][3] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "export", 6) == 0 && cmd->argv[0][6] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "unset", 5) == 0 && cmd->argv[0][5] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "env", 3) == 0 && cmd->argv[0][3] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "exit", 4) == 0 && cmd->argv[0][4] == '\0')
		return (1);
	return (0);
}

/*static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}*/

/*static int	setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout, t_env *env)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	
	if (cmd->redirect_count > 0)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		
		if (*saved_stdin == -1 || *saved_stdout == -1)
		{
			restore_fds(*saved_stdin, *saved_stdout);
			return (1);
		}
		
		if (handle_redirections(cmd, env) != 0)
		{
			restore_fds(*saved_stdin, *saved_stdout);
			return (1);
		}
	}
	return (0);
}*/

int	execute(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv)
		return (FAILURE);
	if (!cmd->argv[0] && cmd->redirect_count > 0)
		return (handle_redirections_only(cmd, env));
	if (!cmd->argv[0])
		return (FAILURE);
	if (cmd->is_piped == 1 || (cmd->next && cmd->next->is_piped == 1))
		return (execute_pipeline(cmd, env));
	else
		return (execute_single_command(cmd, env));
}
