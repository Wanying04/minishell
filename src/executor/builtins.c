#include "minishell.h"

// void	handle_errores(void)
// {
// 	//list situations
// 	return (1);
// }

int	buildin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (!args[i])
	{
		printf("echo: arguments required\n");
		return (1);
	}
	if (args[i] && ft_strncmp(args[i], "-n", 2) == 0 && args[i][2] == '\0')
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	buildin_cd(char **args)
{
	if (!args[1])
	{
		printf("cd: path parameters required\n");
		return (1);
	}
	if (args[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (chdir(args[1] != 0))
	{
		perror("cd");
		return (1);
	}
	return (0);
}

