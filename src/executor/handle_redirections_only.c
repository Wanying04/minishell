#include "minishell.h"

static int	redir_without_command(t_redirect *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT, 0644);
	else if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	else
	{
		close(fd);
		return (SUCCESS);
	}
}

static int	heredoc_without_command(t_redirect *redir)
{
	pid_t	pid;
	int		status;
	char	*line;
	struct sigaction	sa;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		rl_catch_signals = 1;
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				write(2, "minishell: warning: here-document delimited by end-of-file\n", 59);
				break ;
			}
			if (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0
				&& line[ft_strlen(redir->file)] == '\0')
			{
				free(line);
				break ;
			}
			free(line);
		}
		exit(SUCCESS);
	}
	else
	{
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		waitpid(pid, &status, 0);
		setup_signals();
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(1, "\n", 1);
			return (FAILURE);
		}
		return (SUCCESS);
	}
}

int	handle_redirections_only(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			if (heredoc_without_command(&cmd->redirects[i]) != SUCCESS)
				return (FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type != REDIR_HEREDOC)
		{
			if (redir_without_command(&cmd->redirects[i]) != SUCCESS)
				return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}
