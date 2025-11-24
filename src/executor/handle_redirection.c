/*#include "minishell.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

typedef struct s_command
{
	char	**argv;
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

typedef struct s_redir_config
{
	char	*final_input;
	char	*final_output;
	char	**delimiters;
	int		output_mode;
	int		delim_count;
}	t_redir_config;

int	handle_redirections(t_command *cmd, t_env *env)
{
	t_redir_config	redir_config;

	ft_memset(&redir_config, 0, sizeof(redir_config));
	redir_config.delimiters = malloc(sizeof(char *) * cmd->redirect_count);
	if (!redir_config.delimiters)
		return (FAILURE);
	if (analyze_redirections(cmd, &redir_config) != SUCCESS)
	{
		free(redir_config.delimiters);
		return (FAILURE);
	}
	if (execute_redirections(cmd, &redir_config, env) != SUCCESS)
	{
		cleanup_redirections(&redir_config);
		return (FAILURE);
	}
	cleanup_redirections(&redir_config);
	return (SUCCESS);
}

int	analyze_redirections(t_command *cmd, t_redir_config *config)
{
	int	i;
	int	delim_index;

	if (!cmd || !config)
		return (FAILURE);
	i = cmd->redirect_count - 1;
	delim_index = 0;
	while (i >= 0)
	{
		if (!config->final_input && (cmd->redirects[i].type == REDIR_IN || cmd->redirects[i].type == REDIR_HEREDOC))
			config->final_input = cmd->redirects[i].file;
		if (!config->final_output && (cmd->redirects[i].type == REDIR_OUT || cmd->redirects[i].type == REDIR_APPEND))
		{
			config->final_output = cmd->redirects[i].file;
			config->output_mode = (cmd->redirects[i].type == REDIR_APPEND);
		}
		i--;
	}
	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			config->delimiters[delim_index] = cmd->redirects[i].file;
			config->delim_count++;
			delim_index++;
		}
		i++;
	}
	return (SUCCESS);
}

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

void	cleanup_redirections(t_redir_config *config)
{
	int	i;

	if (!config)
	 return ;
	if (config->delimiters)
	{
		i = 0;
		while (i < config->delim_count)
		{
			free(config->delimiters[i]);
			i++;
		}
		free (config->delimiters);
	}
	free(config->final_input);
	free(config->final_output);
}
*/