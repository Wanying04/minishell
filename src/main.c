/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:26:02 by wtang             #+#    #+#             */
/*   Updated: 2025/12/11 13:38:34 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

static void	process_input(char *input, t_env *env)
{
	t_command	*cmd_list;

	if (!*input || is_empty_input(input))
		return ;
	add_history(input);
	cmd_list = parse_input(input, env);
	if (cmd_list)
	{
		env->exit_status = execute(cmd_list, env);
		free_command(cmd_list);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	char	*input;

	(void)argc;
	(void)argv;
	setup_signals();
	env = env_create(envp);
	if (!env)
		return (1);
	while (1)
	{
		g_sigint_received = 0;
		input = readline("minishell$ ");
		if (g_sigint_received)
			env->exit_status = 130;
		if (!input)
		{
			cleanup_and_exit(env);
			break ;
		}
		if (*input != '\0')
			process_input(input, env);
		free(input);
	}
	return (0);
}
