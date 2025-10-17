//HEADER DE WANYING

#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input) // Ctrl+D
		{
			write(1, "exit\n", 5);
			break;
		}
		if (*input)
			add_history(input);
		printf("You typed: %s\n", input);
		free(input);
	}
	rl_clear_history();
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
