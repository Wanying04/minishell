#include "minishell.h"

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
	// Return the read FD of the pipe, do NOT do dup2 here
	return (pipefd[0]);
}

int	process_single_redirection(t_redirect *redir)
{
	if (redir->type == REDIR_HEREDOC)
	{
		// For heredoc, the FD was already created in handle_heredoc_redirection
		if (redir->fd == -1)
			return (FAILURE);
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (FAILURE);
		}
		// Do NOT close the FD here, it might be needed if handle_redirections is called multiple times
		return (SUCCESS);
	}
	else if (redir->type == REDIR_IN)
	{
		redir->fd = open(redir->file, O_RDONLY);
		if (redir->fd == -1)
		{
			perror("minishell");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
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
			perror("minishell");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
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
			perror("minishell");
			return (FAILURE);
		}
		if (dup2(redir->fd, STDOUT_FILENO) == -1)
		{
			perror("minishell");
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

	// Process heredocs only if not yet processed (fd == -1)
	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC && cmd->redirects[i].fd == -1)
		{
			result = handle_heredoc_redirection(cmd->redirects[i].file, env);
			if (result < 0)
				return (FAILURE);
			// Save pipe FD for later use
			cmd->redirects[i].fd = result;
		}
		i++;
	}
	// Apply all redirections (including heredocs)
	i = 0;
	while (i < cmd->redirect_count)
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
		perror("minishell: fork");
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
