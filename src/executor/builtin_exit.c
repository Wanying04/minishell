#include "minishell.h"

static int	is_integer(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_command *cmd)
{
	int	exit_status;

	printf("exit\n");
	if (cmd->argv[1] && cmd->argv[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (FAILURE);
	}
	exit_status = 0;
	if (cmd->argv[1])
	{
		if (!is_integer(cmd->argv[1]))
		{
			printf("minishell: exit: numeric argument required\n");
			exit(2);
		}
		exit_status = ft_atoi(cmd->argv[1]);
	}
	exit_status = exit_status % 256;
	if (exit_status < 0)
		exit_status += 256;
	exit(exit_status);
}
