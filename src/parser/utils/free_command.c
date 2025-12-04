/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:20 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/03 18:08:58 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Libera cmd->redirects.file y cmd->redirects
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
//Libera toda la lista de comandos
void	free_command(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		//Si hay algo en cmd->argv lo libera
		if (cmd->argv)
			ft_free_tokens(cmd->argv);
		//Si hay archivo/s en cmd->redirects libera cmd->redirects.file y el puntero cmd->redirects
		free_redirects(cmd);
		//Si hay dilimitador lo libera (aunque esto igual lo borramos)
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		free(cmd);
		cmd = next;
	}
}
