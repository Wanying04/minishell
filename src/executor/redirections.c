#include "minishell.h"

/*
 * handle_redirections - 处理重定向
 * 
 * 功能：根据 cmd->redirects 设置文件描述符
 * 
 * 输入：t_command *cmd - 包含重定向信息的命令
 * 返回：0 成功，-1 失败
 */
// int	handle_redirections(t_command *cmd)
// {
// 	// TODO: Executor 团队实现
// 	(void)cmd;
// 	return (0);
// }

int	handle_redirections()
{

}

char **env_to_array(t_env *env);
//用来把链表（或哈希表）环境变量转成 char **

reset_signals_to_default()

find_command_path()



void	child_process(t_command *cmd, t_env *env)
{
	char	*path;

	reset_signals_to_default();
	handle_redirections(cmd);
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
		exit(127);
	}
}

int	execute_external_command(t_command *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
		child_process(cmd, env);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	return (status);
}
