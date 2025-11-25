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




int	execute_pipeline(t_command *cmd, t_env *env)
{

}

void	restore_fds(int saved_stdin, int saved_stdout)
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
}

int	setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout, t_env *env)
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
			return (FAILURE);
		}
		if (handle_redirections(cmd, env) != SUCCESS)
		{
			restore_fds(*saved_stdin, *saved_stdout);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	execute_single_command(t_command *cmd, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (setup_redirections(cmd, &saved_stdin, &saved_stdout, env) != SUCCESS)
		return (FAILURE);
	if (is_builtin_command(cmd))
		status = execute_builtins(cmd, env);
	else
		status = execute_external_command(cmd, env);
	restore_fds(&saved_stdin, &saved_stdout);
	return (status);
}

int heredoc_without_command(t_redirect *redirect, t_env *env)
{
	char	*line;
	
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redirect->delimiter) == 0)
		{
			free(line);
			break;
		}
		// 这里只是读取并丢弃输入，不写入任何文件
		free(line);
	}
	return (SUCCESS);
}

int	handle_redirections_only(t_command *cmd, t_env *env)
{
	int	i;
	int	fd;
	int	status;

	i = 0;
	status = SUCCESS;
	while (i < cmd->redirect_count)
	{
		fd = -1;
		if (cmd->redirects[i].type == REDIR_OUT)
			fd = open(cmd->redirects[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (cmd->redirects[i].type == REDIR_APPEND)
			fd = open(cmd->redirects[i].file, O_WRONLY | O_CREAT, 0644);
		else if (cmd->redirects[i].type == REDIR_IN)
			fd = open(cmd->redirects[i].file, O_RDONLY);
		else if (cmd->redirects[i].type == REDIR_HEREDOC)
			status = heredoc_without_command(&cmd->redirects[i], env);
		if (cmd->redirects[i].type != REDIR_HEREDOC)
		{
			if (fd == -1)
			{
				perror("minishell");
				status = FAILURE;
			}
			else
				close(fd);
		}
		// 如果当前操作失败，记录错误但继续处理后续重定向
		if (status == FAILURE)
		{
			// 可以记录第一个错误，但继续处理
			// 或者直接return FAILURE; 取决于你想要的语义
			
		}
		i++;
	}
	return (status);
}

考虑是否要：

遇到第一个错误就返回（更严格）

处理所有重定向，返回最后一个错误（更像bash）

处理所有重定向，返回第一个错误（折中方案）

int	execute(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv)
		return (0);
	if (!cmd->argv[0] && cmd->redirect_count > 0)
		return (handle_redirections_only(cmd, env));
	if (!cmd->argv[0])
		return (0);
	if (cmd->is_piped == 1 || (cmd->next && cmd->next->is_piped == 1))
		return (execute_pipeline(cmd, env));
	else
		return (execute_single_command(cmd, env));
}
