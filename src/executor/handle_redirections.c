#include "minishell.h"

static int	read_heredoc_input(int write_fd, char *delimiter, t_env *env, int dont_expand)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited by end-of-file\n", 59);
			return (SUCCESS);
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0')
		{
			free(line);
			return (SUCCESS);
		}
		expanded = expand_heredoc(line, env, dont_expand);
		write(write_fd, expanded, ft_strlen(expanded));
		write(write_fd, "\n", 1);
		free(line);
		free(expanded);
	}
}

static int	process_heredoc(char *delimiter, t_env *env, int dont_expand)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	struct sigaction	sa;

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		close(pipefd[0]);
		close(pipefd[1]);
		return (FAILURE);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		signal(SIGINT, SIG_DFL);
		rl_catch_signals = 0;
		read_heredoc_input(pipefd[1], delimiter, env, dont_expand);
		close(pipefd[1]);
		exit(SUCCESS);
	}
	else
	{
		close(pipefd[1]);
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		waitpid(pid, &status, 0);
		setup_signals();
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipefd[0]);
			write(1, "\n", 1);
			return (FAILURE);
		}
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(pipefd[0]);
			return (FAILURE);
		}
		close(pipefd[0]);
		return (SUCCESS);
	}
}

static int	open_and_dup(char *file, int flags, int mode, int target_fd)
{
	int	fd;

	fd = -1;
	fd = open(file, flags, mode);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell");
		close(fd);
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}

static int	process_single_redirection(t_redirect *redir, t_env *env, int dont_expand)
{
	if (redir->type == REDIR_IN)
		return (open_and_dup(redir->file, O_RDONLY, 0, STDIN_FILENO));
	if (redir->type == REDIR_HEREDOC)
		return (process_heredoc(redir->file, env, dont_expand));
	if (redir->type == REDIR_OUT)
		return (open_and_dup(redir->file, O_WRONLY | O_CREAT | O_TRUNC,
			0644, STDOUT_FILENO));
	if (redir->type == REDIR_APPEND)
		return (open_and_dup(redir->file, O_WRONLY | O_CREAT | O_APPEND,
			0644, STDOUT_FILENO));
	write(2, "minishell: invalid redirection\n", 31);
	return (FAILURE);
}

int	handle_redirections(t_command *cmd, t_env *env)
{
	int	i;

	i = 0;
	while (i < cmd->redirect_count)
	{
		if (process_single_redirection(&cmd->redirects[i], env, cmd->dont_expand) != SUCCESS)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
