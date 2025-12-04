/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:57:36 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 18:57:37 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(t_command *cmd, t_env *env)
{
	int	i;

	if (!cmd->argv[1])
		return (builtin_env(cmd, env));
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_var_name(cmd->argv[i]))
		{
			printf("minishell: export: not a valid identifier\n");
			return (FAILURE);
		}
		env_set(env, cmd->argv[i]);
		i++;
	}
	return (SUCCESS);
}
