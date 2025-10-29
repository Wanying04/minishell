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
int	execute(t_command *cmd, t_env *env)
{
	int	status;

	// TODO: Executor 团队需要完整实现（管道、重定向、外部命令等）
	// 临时实现：只处理内置命令
	// printf("=== Executor Debug Output ===\n");
	// printf("Would execute command: ");
	if (cmd && cmd->argv && cmd->argv[0])
	{
		// for (int i = 0; cmd->argv[i]; i++)
			// printf("%s ", cmd->argv[i]);
		// printf("\n");
		
		// 检查并执行内置命令
		if (is_builtin_command(cmd->argv))
		{
			status = execute_builtins(cmd->argv, env);
			return (status);
		}
	}
	else
		printf("(no command)\n");
	
	return (0);
}

/*
 * execute_pipeline - 执行管道命令
 * 
 * 功能：处理包含管道的命令序列
 * 
 * 输入：
 *   - t_command *cmd - 管道命令链表
 *   - t_env *env - 环境变量
 * 
 * 返回：最后一个命令的退出状态码
 */
int	execute_pipeline(t_command *cmd, t_env *env)
{
	// TODO: Executor 团队实现
	(void)cmd;
	(void)env;
	return (0);
}

/*
 * handle_redirections - 处理重定向
 * 
 * 功能：根据 cmd->redirects 设置文件描述符
 * 
 * 输入：t_command *cmd - 包含重定向信息的命令
 * 返回：0 成功，-1 失败
 */
int	handle_redirections(t_command *cmd)
{
	// TODO: Executor 团队实现
	(void)cmd;
	return (0);
}

/*
 * is_builtin_command - 检查是否为内置命令
 * 
 * 功能：判断给定命令是否为 shell 内置命令
 * 
 * 输入：char **argv - 命令参数数组
 * 返回：1 是内置命令，0 不是
 */
int	is_builtin_command(char **argv)
{
	if (!argv || !argv[0])
		return (0);
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (1);
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (1);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(argv[0], "export", 7) == 0)
		return (1);
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		return (1);
	if (ft_strncmp(argv[0], "env", 4) == 0)
		return (1);
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (1);
	return (0);
}

// ========== 以下是旧代码，保留作为参考 ==========
// int	execute(char **args, t_env *env)
// {
// 	pid_t	pid;
// 	int		status;

// 	if (is_builtin(args[0]))
// 		return (execute_builtins(args, env));
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		execve(path, args, env);
// 		exit(127);
// 	}
// 	else if (pid > 0)
// 	{
// 		waitpid(pid, &status, 0);
// 		return (WEXITSTATUS(status));
// 	}
// 	return (-1);
// }

// int	is_builtin()
