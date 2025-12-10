/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:53:35 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 20:53:36 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(t_command *cmd)
{
	int	exit_status;

	if (!cmd->prev)
		write(1, "exit\n", 5);
	exit_status = 0;
	if (cmd->argv[1])
	{
		if (!is_integer(cmd->argv[1]) || !ft_atol(cmd->argv[1], &exit_status))
		{
			write(2, "minishell: exit: numeric argument required\n", 43);
			exit(2);
		}
	}
	if (cmd->argv[1] && cmd->argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (FAILURE);
	}
	exit_status = exit_status % 256;
	if (exit_status < 0)
		exit_status += 256;
	exit(exit_status);
}
