/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:25 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 20:52:26 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_unset(t_command *cmd, t_env *env)
{
	int	i;

	if (!cmd->argv[1])
		return (SUCCESS);
	i = 1;
	while (cmd->argv[i])
	{
		if (is_valid_var_name(cmd->argv[i]))
			env_unset(env, cmd->argv[i]);
		i++;
	}
	return (SUCCESS);
}
