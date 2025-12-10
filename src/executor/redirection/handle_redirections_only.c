/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections_only.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 19:17:17 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:39:34 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_read_loop(const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: heredoc delimited by EOF\n", 45);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\0')
		{
			free(line);
			break ;
		}
		free(line);
	}
	return (0);
}

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
		heredoc_read_loop(redir->file);
		exit(SUCCESS);
	}
	ignore_sigint();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		return (FAILURE);
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
