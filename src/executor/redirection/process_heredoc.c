/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 19:44:41 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:15:37 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_parent(int read_fd, pid_t pid, t_env *env,
	int should_dup)
{
	int	status;

	ignore_sigint();
	waitpid(pid, &status, 0);
	setup_signals();
	rl_on_new_line();
	rl_replace_line("", 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		close(read_fd);
		env->exit_status = 130;
		return (FAILURE);
	}
	if (should_dup)
	{
		if (dup2(read_fd, STDIN_FILENO) == -1)
		{
			perror("minishell");
			close(read_fd);
			return (FAILURE);
		}
	}
	close(read_fd);
	return (SUCCESS);
}

static int	read_heredoc_input(int write_fd, char *delimiter,
	t_env *env, int dont_expand)
{
	char	*line;
	char	*expanded;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: heredoc delimited by EOF\n", 45);
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

static void	process_heredoc_child(int write_fd, char *delimiter,
	t_env *env, int dont_expand)
{
	signal(SIGINT, SIG_DFL);
	rl_catch_signals = 1;
	dup2(STDERR_FILENO, STDOUT_FILENO);
	read_heredoc_input(write_fd, delimiter, env, dont_expand);
	close(write_fd);
	exit(SUCCESS);
}

int	process_heredoc(char *delimiter, t_env *env,
	int dont_expand, int should_dup)
{
	int					pipefd[2];
	pid_t				pid;

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
		process_heredoc_child(pipefd[1], delimiter, env, dont_expand);
	else
	{
		close(pipefd[1]);
		return (process_heredoc_parent(pipefd[0], pid, env, should_dup));
	}
	return (SUCCESS);
}
