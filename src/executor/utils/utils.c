/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:00:27 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 21:19:34 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_ll_overflow(long long number, int sign, char digit)
{
	if (number > LLONG_MAX / 10)
		return (0);
	if (number == LLONG_MAX / 10)
	{
		if (sign == 1 && (digit - '0') > LLONG_MAX % 10)
			return (0);
		if (sign == -1 && (digit - '0') > (LLONG_MAX % 10) + 1)
			return (0);
	}
	return (1);
}

static int	ft_str_to_ll(const char *nptr, long long *number)
{
	int		i;
	int		sign;

	i = 0;
	*number = 0;
	sign = 1;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i])
	{
		if (!check_ll_overflow(*number, sign, nptr[i]))
			return (0);
		*number = 10 * (*number) + (nptr[i] - '0');
		i++;
	}
	*number = *number * sign;
	return (1);
}

int	ft_atol(const char *nptr, int *value)
{
	long long	number;
	int			ok;

	ok = ft_str_to_ll(nptr, &number);
	if (!ok)
		return (0);
	*value = (int)number;
	return (1);
}

static int	is_valid_arg(char *arg)
{
	if (arg[0] != '\0')
		return (1);
	if (ft_strlen(arg) == 2 && (arg[0] == '\x01' || arg[0] == '\x02'))
		return (1);
	return (0);
}

void	cleanup_empty_args(t_command *cmd)
{
	int		i;
	int		j;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		if (is_valid_arg(cmd->argv[i]))
		{
			if (i != j)
				cmd->argv[j] = cmd->argv[i];
			j++;
		}
		else
			free(cmd->argv[i]);
		i++;
	}
	cmd->argv[j] = NULL;
}
