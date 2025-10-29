#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_sigint_received = 1;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, handle_signal);  /* Ctrl+C */
	signal(SIGQUIT, SIG_IGN);       /* Ctrl+\ */
}

static int	cleanup_and_exit(t_env *env)
{
	write(1, "exit\n", 5);
	rl_clear_history();
	env_destroy(env);
	return (1);
}

static void	process_input(char *input, t_env *env)
{
	t_command	*cmd_list;

	if (!*input)
		return ;
	add_history(input);
	cmd_list = parse_input(input);
	if (cmd_list)
	{
		execute(cmd_list, env);
		free_command(cmd_list);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*env;

	(void)argc;
	(void)argv;
	setup_signals();
	env = env_create(envp);
	while (1)
	{
		g_sigint_received = 0;
		input = readline("minishell$ ");
		if (!input && cleanup_and_exit(env))
			break ;
		if (input)
			process_input(input, env);
		free(input);
	}
	return (0);
}

/* void	process_input(char *input)
{
	char	**tokens;

	if (*input)
	{
		add_history(input);
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			write(1, "exit\n", 5);
			rl_clear_history();
			exit(0);
		}
		tokens = ft_split_tokens(input);
		if (tokens)
		{
			for (int i = 0; tokens[i]; i++)
				printf("token[%d]: %s\n", i, tokens[i]);
			ft_free_tokens(tokens);
		}
	}
	free(input);
} */