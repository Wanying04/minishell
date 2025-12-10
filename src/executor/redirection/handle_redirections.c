/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 19:44:47 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 20:20:29 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_single_redirection(t_redirect *redir, t_env *env,
	int dont_expand, int should_dup)
{
	if (redir->file && redir->file[0] == '$' && !is_integer(&redir->file[0]))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		return (FAILURE);
	}
	if (redir->type == REDIR_HEREDOC)
		return (process_heredoc(redir->file, env, dont_expand, should_dup));
	return (handle_file_redirection(redir, should_dup));
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
		if (cmd->redirects[i].type == REDIR_IN
			|| cmd->redirects[i].type == REDIR_HEREDOC)
			last_input_redir = i;
		i++;
	}
	i = 0;
	while (i++ < cmd->redirect_count)
	{
		should_dup = 1;
		if ((cmd->redirects[i].type == REDIR_IN || cmd->redirects[i].type
				== REDIR_HEREDOC) && i != last_input_redir)
			should_dup = 0;
		if (process_single_redirection(&cmd->redirects[i], env,
				cmd->dont_expand, should_dup) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
