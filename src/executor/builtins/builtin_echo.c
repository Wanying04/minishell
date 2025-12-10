/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:55:51 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 20:56:37 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static void	echo_print_args(t_command *cmd, int i, int newline)
{
	while (cmd->argv[i])
	{
		if (ft_strlen(cmd->argv[i]) == 2
			&& (cmd->argv[i][0] == '\x01' || cmd->argv[i][0] == '\x02'))
		{
			if (cmd->argv[i + 1])
				write(1, " ", 1);
		}
		else
		{
			write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
			if (cmd->argv[i + 1])
				write(1, " ", 1);
		}
		i++;
	}
	if (newline)
		write(1, "\n", 1);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (cmd->argv[i] && is_n_option(cmd->argv[i]))
	{
		newline = 0;
		i++;
	}
	echo_print_args(cmd, i, newline);
	return (SUCCESS);
}
