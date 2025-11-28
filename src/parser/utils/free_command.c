/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:20 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirects(t_command *cmd)
{
	int	i;

	if (cmd->redirects)
	{
		i = 0;
		while (i < cmd->redirect_count)
		{
			if (cmd->redirects[i].file)
				free(cmd->redirects[i].file);
			i++;
		}
		free(cmd->redirects);
	}
}

void	free_command(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
			ft_free_tokens(cmd->argv);
		free_redirects(cmd);
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		free(cmd);
		cmd = next;
	}
}
