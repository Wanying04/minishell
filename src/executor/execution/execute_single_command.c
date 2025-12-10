/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:33:22 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 18:33:41 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtins(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FAILURE);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0 && cmd->argv[0][4] == '\0')
		return (builtin_echo(cmd));
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0 && cmd->argv[0][2] == '\0')
		return (builtin_cd(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0 && cmd->argv[0][3] == '\0')
		return (builtin_pwd(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "export", 6) == 0
		&& cmd->argv[0][6] == '\0')
		return (builtin_export(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "unset", 5) == 0
		&& cmd->argv[0][5] == '\0')
		return (builtin_unset(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0 && cmd->argv[0][3] == '\0')
		return (builtin_env(cmd, env));
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0
		&& cmd->argv[0][4] == '\0')
		return (builtin_exit(cmd));
	else if (ft_strncmp(cmd->argv[0], ".", 1) == 0 && cmd->argv[0][1] == '\0')
		return (builtin_dot(cmd));
	else
		return (FAILURE);
}

int	is_builtin_command(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FAILURE);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0 && cmd->argv[0][4] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "cd", 2) == 0 && cmd->argv[0][2] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0 && cmd->argv[0][3] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "export", 6) == 0 && cmd->argv[0][6] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "unset", 5) == 0 && cmd->argv[0][5] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "env", 3) == 0 && cmd->argv[0][3] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], "exit", 4) == 0 && cmd->argv[0][4] == '\0')
		return (SUCCESS);
	if (ft_strncmp(cmd->argv[0], ".", 1) == 0 && cmd->argv[0][1] == '\0')
		return (SUCCESS);
	return (FAILURE);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}

static int	setup_redirections(t_command *cmd, int *saved_stdin,
		int *saved_stdout, t_env *env)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	if (cmd->redirect_count > 0)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		if (*saved_stdin == -1 || *saved_stdout == -1)
		{
			restore_fds(*saved_stdin, *saved_stdout);
			return (FAILURE);
		}
		if (handle_redirections(cmd, env) != SUCCESS)
		{
			restore_fds(*saved_stdin, *saved_stdout);
			return (FAILURE);
		}
	}
	return (SUCCESS);
}

int	execute_single_command(t_command *cmd, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (is_builtin_command(cmd) == SUCCESS)
	{
		if (setup_redirections(cmd, &saved_stdin, &saved_stdout, env)
			!= SUCCESS)
			return (FAILURE);
		status = execute_builtins(cmd, env);
		restore_fds(saved_stdin, saved_stdout);
	}
	else
		status = execute_external_command(cmd, env);
	return (status);
}
