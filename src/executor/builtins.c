#include "minishell.h"

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->argv[i] && ft_strncmp(cmd->argv[i], "-n", 2) == 0 && cmd->argv[i][2] == '\0')
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_pwd(t_command *cmd)
{
	char	cwd[4096];

	(void)cmd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

int	is_integer(char *str)
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

	if (cmd->argv[1] && cmd->argv[2])
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	exit_status = 0;
	if (cmd->argv[1])
	{
		if (!is_integer(cmd->argv[1]))
		{
			printf("exit: numeric argument required\n");
			exit(2);
		}
		exit_status = ft_atoi(cmd->argv[1]);
	}
	exit_status = exit_status % 256;
	if (exit_status < 0)
		exit_status += 256;
	printf("exit\n");
	exit(exit_status);
}

int	execute_builtins(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0 && cmd->argv[0][4] == '\0')
		return (builtin_echo(cmd));
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0 && cmd->argv[0][2] == '\0')
		return (builtin_cd(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0 && cmd->argv[0][3] == '\0')
		return (builtin_pwd(cmd));
	else if (ft_strncmp(cmd->argv[0], "export", 6) == 0 && cmd->argv[0][6] == '\0')
		return (builtin_export(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0 && cmd->argv[0][5] == '\0')
		return (builtin_unset(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0 && cmd->argv[0][3] == '\0')
		return (builtin_env(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0 && cmd->argv[0][4] == '\0')
		return (builtin_exit(cmd));
	else
		return (1);
}
