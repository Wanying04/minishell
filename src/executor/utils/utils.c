#include "minishell.h"

int	ft_atol(const char *nptr, int *value)
{
	int			i;
	long long	number;
	int			sign;

	i = 0;
	number = 0;
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
		if (number > LLONG_MAX / 10)
			return (0);
		if (number == LLONG_MAX / 10)
		{
			if (sign == 1 && (nptr[i] - '0') > LLONG_MAX % 10)
				return (0);
			if (sign == -1 && (nptr[i] - '0') > (LLONG_MAX % 10) + 1)
				return (0);
		}
		number = 10 * number + (nptr[i] - '0');
		i++;
	}
	*value = (int)(number * sign);
	return (1);
}

void	cleanup_empty_args(t_command *cmd)
{
	int	i;
	int	j;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] != '\0' || 
			(ft_strlen(cmd->argv[i]) == 2 && 
			(cmd->argv[i][0] == '\x01' || cmd->argv[i][0] == '\x02')))
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

int	is_a_directory(const char *path)
{
	struct stat	path_stat;

	if (!path)
		return (0);
	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}
