/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file_redirection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:15:14 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:39:33 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_and_dup(t_dupinfo info)
{
	int	fd;

	fd = open(info.file, info.flags, info.mode);
	if (fd == -1)
	{
		perror("minishell");
		return (FAILURE);
	}
	if (info.should_dup)
	{
		if (dup2(fd, info.target_fd) == -1)
		{
			perror("minishell");
			close(fd);
			return (FAILURE);
		}
	}
	close(fd);
	return (SUCCESS);
}

static int	handle_redir_in(t_redirect *redir, int should_dup)
{
	t_dupinfo	info;

	info.file = redir->file;
	info.flags = O_RDONLY;
	info.mode = 0;
	info.target_fd = STDIN_FILENO;
	info.should_dup = should_dup;
	return (open_and_dup(info));
}

static int	handle_redir_out(t_redirect *redir)
{
	t_dupinfo	info;

	info.file = redir->file;
	info.flags = O_WRONLY | O_CREAT | O_TRUNC;
	info.mode = 0644;
	info.target_fd = STDOUT_FILENO;
	info.should_dup = 1;
	return (open_and_dup(info));
}

static int	handle_redir_append(t_redirect *redir)
{
	t_dupinfo	info;

	info.file = redir->file;
	info.flags = O_WRONLY | O_CREAT | O_APPEND;
	info.mode = 0644;
	info.target_fd = STDOUT_FILENO;
	info.should_dup = 1;
	return (open_and_dup(info));
}

int	handle_file_redirection(t_redirect *redir, int should_dup)
{
	if (redir->type == REDIR_IN)
		return (handle_redir_in(redir, should_dup));
	if (redir->type == REDIR_OUT)
		return (handle_redir_out(redir));
	if (redir->type == REDIR_APPEND)
		return (handle_redir_append(redir));
	write(2, "minishell: invalid redirection\n", 31);
	return (FAILURE);
}
