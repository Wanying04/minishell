#include "minishell.h"

char **env_to_array(t_env *env)
{
	char	**env_array;
	int		i;

	if (!env || !env->vars)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env->count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		env_array[i] = ft_strdup(env->vars[i]);
		if (!env_array[i])
		{
			while (--i >= 0)
				free(env_array[i]);
			free(env_array);
			return (NULL);
		}
		i++;
	}
	env_array[env->count] = NULL;
	return (env_array);
}

char	*search_in_directory(char *dir, char *cmd)
{
	char	full_path[1024];

	ft_strlcpy(full_path, dir, sizeof(full_path));
	ft_strlcat(full_path, "/", sizeof(full_path));
	ft_strlcat(full_path, cmd, sizeof(full_path));
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

char	*search_in_path(char *cmd, char *path_env)
{
	char	*path_copy;
	char	*current;
	char	*end;
	char	*result;
	char	saved_char;

	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	current = path_copy;
	while (*current)
	{
		end = current;
		while (*end && *end != ':')
			end++;
		saved_char = *end;
		*end = '\0';
		result = search_in_directory(current, cmd);
		*end = saved_char;
		if (result)
		{
			free(path_copy);
			return (result);
		}
		if (*end == ':')
			current = end + 1;
		else
			current = end;
	}
	free(path_copy);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = env_get(env, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	result = search_in_path(cmd, path_env);
	return (result);
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

int	handle_heredoc_redirection(char *delimiter, t_env *env)
{
	int		pipefd[2];
	char	*line;
	char	*expanded;

	if (pipe(pipefd) == -1)
	{
		write(2, "minishell: pipe creation failed\n", 32);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited by end-of-file\n", 59);
			break ;		
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 && line[ft_strlen(delimiter)] == '\0')
		{
			free (line);
			break ;
		}
		expanded = expand_heredoc(line, env);
		write(pipefd[1], expanded, ft_strlen(expanded));
		write(pipefd[1], "\n", 1);
		free(line);
		free(expanded);
	}
	close (pipefd[1]);
	// Retornar el FD de lectura del pipe, NO hacer dup2 aquí
	return (pipefd[0]);
}

int	process_single_redirection(t_redirect *redir)
{
	if (redir->type == REDIR_HEREDOC)
	{
		// Para heredoc, el FD ya fue creado en handle_heredoc_redirection
		if (redir->fd == -1)
			return (FAILURE);
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (FAILURE);
		}
		// NO cerrar el FD aquí, podría necesitarse si handle_redirections se llama múltiples veces
		return (SUCCESS);
	}
	else if (redir->type == REDIR_IN)
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

int	handle_redirections(t_command *cmd, t_env *env)
{
	int	i;
	int	result;

	// Procesar heredocs solo si aún no han sido procesados (fd == -1)
	i = 0;
	while(i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC && cmd->redirects[i].fd == -1)
		{
			result = handle_heredoc_redirection(cmd->redirects[i].file, env);
			if (result < 0)
				return (FAILURE);
			// Guardar el FD del pipe para usarlo después
			cmd->redirects[i].fd = result;
		}
		i++;
	}
	// Aplicar todas las redirecciones (incluyendo heredocs)
	i = 0;
	while(i < cmd->redirect_count)
	{
		result = process_single_redirection(&cmd->redirects[i]);
		if (result != SUCCESS)
			return (FAILURE);
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
	if (handle_redirections(cmd, env) != SUCCESS)
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
	{
		child_process(cmd, env);
		exit (EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
			return (128 + WTERMSIG(status));
		}
		else
			return (FAILURE);
	}
}
