#include "minishell.h"
#include <signal.h>
#include <termios.h>

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

void	process_input(char *input)
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
}

int	main(void)
{
	char	*input;

	setup_signals();
	while (1)
	{
		g_sigint_received = 0;
		input = readline("minishell$ ");
		if (!input)  /* Ctrl+D o Ctrl+C */
		{
			write(1, "exit\n", 5);  /* Si fue Ctrl+D, salir */
			rl_clear_history();
			return (0);
		}
		process_input(input);
	}
	return (0);
}

/* int main(void)
{
    // 1. Configuración inicial
    setup_signals();       // Configurar manejo de señales (Ctrl+C, etc.)
    init_environment();    // Inicializar variables de entorno
    setup_terminal();      // Configurar terminal si es necesario
    
    // 2. Bucle principal (REPL)
    while (!should_exit) {
        // Mostrar prompt
        display_prompt();
        
        // Leer entrada del usuario
        char *input = read_input();
        
        // Procesar el comando (aquí es donde entra tu pipeline)
        if (input != NULL) {
            execute_input(input);  // ← Aquí comienza Parser → Executor
        }
    }
    
    // 3. Limpieza antes de salir
    cleanup();
    return (0);
}*/
