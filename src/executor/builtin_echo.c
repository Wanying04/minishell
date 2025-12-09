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
	while (cmd->argv[i])
	{
		// Si el argumento solo contiene marcadores de comillas vacías, no imprimir contenido
		if (!(ft_strlen(cmd->argv[i]) == 2 && 
			(cmd->argv[i][0] == '\x01' || cmd->argv[i][0] == '\x02')))
		{
			write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
		}
		if (cmd->argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (SUCCESS);
}
