/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:53:35 by wtang             #+#    #+#             */
/*   Updated: 2025/12/11 13:45:48 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*get_cmd_list_head(t_command *cmd)
{
	t_command	*head;

	head = cmd;
	while (head->prev)
		head = head->prev;
	return (head);
}

int	builtin_exit(t_command *cmd, t_env *env)
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
	free_command(get_cmd_list_head(cmd));
	rl_clear_history();
	env_destroy(env);
	exit(exit_status);
}
