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

static int	process_heredoc(char *delimiter, t_env *env, int dont_expand, int should_dup)
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
		rl_catch_signals = 1;
		dup2(STDERR_FILENO, STDOUT_FILENO);
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
		rl_on_new_line();
		rl_replace_line("", 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(STDERR_FILENO, "\n", 1);
			close(pipefd[0]);
			env->exit_status = 130;
			return (FAILURE);
		}
		if (should_dup)
		{
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
			{
				perror("minishell");
				close(pipefd[0]);
				return (FAILURE);
			}
		}
		close(pipefd[0]);
		return (SUCCESS);
	}
}

static int	open_and_dup(char *file, int flags, int mode, int target_fd, int should_dup)
{
	int	fd;

	fd = -1;
	fd = open(file, flags, mode);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (should_dup)
	{
		if (dup2(fd, target_fd) == -1)
		{
			perror("minishell");
			close(fd);
			return (FAILURE);
		}
	}
	close(fd);
	return (SUCCESS);
}

static int	process_single_redirection(t_redirect *redir, t_env *env, int dont_expand, int should_dup)
{
	// 检查文件名是否以 $ 开头（表示是未展开的变量名，即 ambiguous redirect）
	if (redir->file && redir->file[0] == '$')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (FAILURE);
	}
	if (redir->type == REDIR_IN)
		return (open_and_dup(redir->file, O_RDONLY, 0, STDIN_FILENO, should_dup));
	if (redir->type == REDIR_HEREDOC)
		return (process_heredoc(redir->file, env, dont_expand, should_dup));
	if (redir->type == REDIR_OUT)
		return (open_and_dup(redir->file, O_WRONLY | O_CREAT | O_TRUNC,
			0644, STDOUT_FILENO, 1));
	if (redir->type == REDIR_APPEND)
		return (open_and_dup(redir->file, O_WRONLY | O_CREAT | O_APPEND,
			0644, STDOUT_FILENO, 1));
	write(2, "minishell: invalid redirection\n", 31);
	return (FAILURE);
}

int	handle_redirections(t_command *cmd, t_env *env)
{
	int	i;
	int	last_input_redir;
	int	should_dup;

	i = 0;
	last_input_redir = -1;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_IN || cmd->redirects[i].type == REDIR_HEREDOC)
			last_input_redir = i;
		i++;
	}
	i = 0;
	while (i < cmd->redirect_count)
	{
		should_dup = 1;
		if ((cmd->redirects[i].type == REDIR_IN || cmd->redirects[i].type == REDIR_HEREDOC)
			&& i != last_input_redir)
			should_dup = 0;
		if (process_single_redirection(&cmd->redirects[i], env, cmd->dont_expand, should_dup) != SUCCESS)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
