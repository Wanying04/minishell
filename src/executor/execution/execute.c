/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:23:25 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 18:23:26 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv)
		return (SUCCESS);
	cleanup_empty_args(cmd);
	if (!cmd->argv[0] && cmd->redirect_count > 0)
		return (handle_redirections_only(cmd));
	if (!cmd->argv[0])
		return (SUCCESS);
	if (cmd->next != NULL)
		return (execute_pipeline(cmd, env));
	else
		return (execute_single_command(cmd, env));
}
