/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:58:39 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 21:56:31 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_pipeline(t_command *cmd)
{
	while (cmd)
	{
		if (!cmd->argv || !cmd->argv[0])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\\n",
				STDERR_FILENO);
			return (FAILURE);
		}
		cmd = cmd->next;
	}
	return (SUCCESS);
}

int	execute_pipeline(t_command *cmd, t_env *env)
{
	if (is_valid_pipeline(cmd) != SUCCESS)
		return (SYNTAX_ERROR);
	return (execute_pipeline_core(cmd, env));
}
