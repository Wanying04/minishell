#include "minishell.h"

/*
 * execute - 主执行函数
 * 
 * 功能：执行解析后的命令链表
 * 
 * 输入：
 *   - t_command *cmd - 要执行的命令链表
 *   - t_env *env - 环境变量
 * 
 * 返回：命令的退出状态码
 * 
 * 执行流程：
 * 1. 检查是否为内置命令
 * 2. 处理重定向
 * 3. 如果有管道，调用 execute_pipeline
 * 4. 否则，fork 并 execve
 */

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

int	execute(t_command *cmd, t_env *env)
{
	int	status;
	int	saved_stdin;

	status = 0;
	if (!cmd || !cmd->argv)
		return (0);
	if (!cmd->argv[0])
	{
		if (cmd->redirect_count > 0)
		{
			saved_stdin = dup(STDIN_FILENO);
			status = handle_redirections(cmd);
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (status);
		}
		return (0);
	}
	if (!cmd->next)
	{
		if (is_builtin_command(cmd))
			status = execute_builtins(cmd, env);
		else
			status = execute_external_command(cmd, env);
	}
	// else
	// {
	// 	status = handle_pipes(cmd, env);
	// }
	return (status);
}
