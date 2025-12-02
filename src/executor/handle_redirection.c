#include "minishell.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

typedef struct s_command
{
	char	**argv;
	int		is_piped;
	int		is_builtin;
	int		redirect_count;
	t_redirect	*redirects;
	struct s_command	*prev;
	struct s_command	*next;
}	t_command;

typedef struct s_redirect
{
	int		type;
	char	*file;
}	t_redirect;

int	execute_redirections(t_command *cmd, t_redir_config *config, t_env *env)
{
	int	i;

	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_IN)
		{
			if (process_redir_in(&cmd->redirects[i]) != SUCCESS)
				return (FAILURE);
		}
		else if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			if (process_redir_heredoc(&cmd->redirects[i], env) != SUCCESS)
				return (FAILURE);
		}
		else if (cmd->redirects[i].type == REDIR_OUT)
		{
			if (process_redir_out(&cmd->redirects[i]) != SUCCESS)
				return (FAILURE);
		}
		else if (cmd->redirects[i].type == REDIR_APPEND)
		{
			if (process_redir_append(&cmd->redirects[i]) != SUCCESS)
				return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

int	process_redir_in(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		close (fd);
		return (FAILURE);
	}
	close (fd);
	return (SUCCESS);
}

int	process_redir_heredoc(t_redirect *redir, t_env *env)
{
	int		pipefd[2];
	char	*line;
	char	*expanded;

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited by end-of-file\n", 59);
			break ;		
		}
		if (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0 && line[ft_strlen(redir->file)] == '\0')
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
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell");
		close(pipefd[0]);
		return (FAILURE);
	}
	close (pipefd[0]);
	return (SUCCESS);
}

int	process_redir_out(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}

int	process_redir_append(t_redirect *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		close(fd);
		return (FAILURE);
	}
	close(fd);
	return (SUCCESS);
}
