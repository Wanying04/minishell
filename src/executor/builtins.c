#include "minishell.h"

int	builtin_echo(char **args)
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

int	builtin_cd(char **args)
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

int	builtin_pwd(char **args)
{
	char	cwd[4096];

	(void)args;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

int	builtin_exit(char **args)
{
	int	exit_status;

	exit_status = 0;
	if (args[1])
		exit_status = ft_atoi(args[1]);
	printf("exit\n");
	exit(exit_status);
}

int	execute_builtins(char **args)
{
	if (!args || !args[0])
		return (1);
	if (ft_strncmp(args[0], "echo", 5) == 0 && args[0][4] == '\0')
		return (builtin_echo(args));
	else if (ft_strncmp(args[0], "cd", 3) == 0 && args[0][2] == '\0')
		return (builtin_cd(args));
	else if (ft_strncmp(args[0], "pwd", 4) == 0 && args[0][3] == '\0')
		return (builtin_pwd(args));
	else if (ft_strncmp(args[0], "export", 7) == 0 && args[0][6] == '\0')
		return (builtin_export(args));
	else if (ft_strncmp(args[0], "unset", 6) == 0 && args[0][5] == '\0')
		return (builtin_unset(args));
	else if (ft_strncmp(args[0], "env", 4) == 0 && args[0][3] == '\0')
		return (builtin_env(args));
	else if (ft_strncmp(args[0], "exit", 5) == 0 && args[0][4] == '\0')
		return (builtin_exit(args));
	else
		return (1);
}
