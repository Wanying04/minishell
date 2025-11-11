#include "minishell.h"


char **env_to_array(t_env *env);
//用来把链表（或哈希表）环境变量转成 char **



find_command_path()
{

}

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

int	process_single_redirection(t_redirect *redir)
{
	if (redir->type == REDIR_IN)
	{
		open(redir->file, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir->type == REDIR_OUT)
	{
			open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		dup2(fd, STDERR_FILENO);
		close(fd);
	}
	return (0);
}

int	handle_heredoc_redirection(char *delimiter)
{
Heredoc (<<) 实现步骤：

pipe(pipefd) 创建管道

循环读取用户输入：

显示提示符

读取一行输入

如果输入 == 分隔符：退出循环

否则：写入管道

关闭管道写端

dup2(pipefd[0], STDIN_FILENO) 重定向输入

关闭管道读端
}

int	handle_redirections(t_command *cmd)
{
	int	i;
	int	result;

	i = 0;
	while(i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			result = handle_heredoc_redirection(cmd->redirects[i].file);
			if (result != 1)
				return (0);
		}
		i++;
	}
	i = 0;
	while(i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type != REDIR_HEREDOC)
		{
			result = process_single_redirection(&cmd->redirects[i]);
			if (result != 1)
				return (0);
		}
		i++;
	}
	return (1);
}

void	reset_signals_to_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

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
