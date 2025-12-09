#include "minishell.h"

int	builtin_dot(t_command *cmd)
{
	if (!cmd->argv[1])
	{
		write(2, ".: filename argument required\n", 30);
		write(2, ".: usage: . filename [arguments]\n", 33);
		return (2);
	}
	write(2, ".: ", 3);
	write(2, cmd->argv[1], ft_strlen(cmd->argv[1]));
	write(2, ": file not found\n", 17);
	return (1);
}
