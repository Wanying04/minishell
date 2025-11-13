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
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

static int	cleanup_and_exit(t_env *env)
{
	write(1, "exit\n", 5);
	rl_clear_history();
	env_destroy(env);
	return (1);
}

int	is_empty_input(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}

static void	process_input(char *input, t_env *env)
{
	t_command	*cmd_list;

	if (!*input || is_empty_input(input))
		return ;
	add_history(input);
	cmd_list = parse_input(input, env);
	if (cmd_list)
	{
		/* If PARSER_DEBUG is set, print the parsed structure instead of executing */
		if (getenv("PARSER_DEBUG"))
		{
			print_command_list(cmd_list);
			free_command(cmd_list);
		}
		else
		{
			execute(cmd_list, env);
			free_command(cmd_list);
		}
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
