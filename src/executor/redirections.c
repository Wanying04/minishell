#include "minishell.h"

char **env_to_array(t_env *env);
//用来把链表（或哈希表）环境变量转成 char **

find_command_path()
{

}

// Heredoc (<<) 实现步骤：
// pipe(pipefd) 创建管道
// 循环读取用户输入：
// 显示提示符
// 读取一行输入
// 如果输入 == 分隔符：退出循环
// 否则：写入管道
// 关闭管道写端
// dup2(pipefd[0], STDIN_FILENO) 重定向输入
// 关闭管道读端

// 创建临时文件或管道来存储输入内容
// 循环读取用户输入，直到遇到单独的分隔符行
// 将内容写入临时文件
// 重定向标准输入到这个临时文件

int	handle_heredoc_redirection(char *delimiter)
{
	int		fd;
	char	*line;

	while(1)
	{
		while()
	}
}

int	process_single_redirection(t_redirect *redir)
{
	if (redir->type == REDIR_IN)
	{
		redir->fd = open(redir->file, O_RDONLY);
		if (redir->fd == -1)
		{
			perror("open");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(redir->fd);
			return (FAILURE);
		}
		close(redir->fd);
	}
	else if (redir->type == REDIR_OUT)
	{
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (redir->fd == -1)
		{
			perror("open");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(redir->fd);
			return (FAILURE);
		}
		close(redir->fd);
	}
	else if (redir->type == REDIR_APPEND)
	{
		redir->fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (redir->fd == -1)
		{
			perror("open");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(redir->fd);
			return (FAILURE);
		}
		close(redir->fd);
	}
	else
	{
		write(2, "minishell: invalid redirection\n", 31);
		return (FAILURE);
	}
	return (SUCCESS);
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
			if (result != SUCCESS)
				return (FAILURE);
		}
		i++;
	}
	i = 0;
	while(i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type != REDIR_HEREDOC)
		{
			result = process_single_redirection(&cmd->redirects[i]);
			if (result != SUCCESS)
				return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
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
	if (handle_redirections(cmd) != SUCCESS)
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

int	execute_external_command(t_command *cmd, t_env *env)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (FAILURE);
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
		else
			return (FAILURE);
	}
}
