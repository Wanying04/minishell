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
	char	*line;

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
	return (SUCCESS);
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
