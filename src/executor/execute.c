#include "minishell.h"

/*
 * execute - 主执行函数
 * 
 * 功能：执行解析后的命令链表
 * 
 * 输入：
 *   - t_command *cmd - 要执行的命令链表
 *   - t_env *env - 环境变量
 * 
 * 返回：命令的退出状态码
 * 
 * 执行流程：
 * 1. 检查是否为内置命令
 * 2. 处理重定向
 * 3. 如果有管道，调用 execute_pipeline
 * 4. 否则，fork 并 execve
 */

int	is_builtin_command(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (ft_strncmp(cmd->argv[0], "echo", 4) == 0 && cmd->argv[0][4] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "cd", 2) == 0 && cmd->argv[0][2] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0 && cmd->argv[0][3] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "export", 6) == 0 && cmd->argv[0][6] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "unset", 5) == 0 && cmd->argv[0][5] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "env", 3) == 0 && cmd->argv[0][3] == '\0')
		return (1);
	if (ft_strncmp(cmd->argv[0], "exit", 4) == 0 && cmd->argv[0][4] == '\0')
		return (1);
	return (0);
}

int	execute(t_command *cmd, t_env *env)
{
	int			status;
	int			saved_stdin;
	t_command	*current;

	status = 0;
	current = cmd;
	
	/*
	 * Recorrer toda la lista de comandos:
	 * 
	 * - Comandos separados por ';' → Se ejecutan secuencialmente (este bucle)
	 *   Ejemplo: "echo hola ; echo mundo"
	 *   Lista: [cmd1: echo hola] → [cmd2: echo mundo]
	 *   Ejecución: cmd1, esperar, cmd2, esperar
	 * 
	 * - Comandos conectados por '|' → Se detectan aquí y se delegan a execute_pipeline()
	 *   Ejemplo: "echo hola | cat | wc"
	 *   Lista: [cmd1: echo] → [cmd2: cat] → [cmd3: wc]
	 *   TODO: Detectar que cmd->next forma un pipeline y llamar:
	 *         execute_pipeline(current, env);
	 *   
	 * Nota: El parser NO diferencia entre ; y | en la lista enlazada.
	 *       Ambos crean una lista: cmd1 → cmd2 → cmd3
	 *       El executor debe determinar si hay pipes mirando algún flag
	 *       o implementar lógica en execute_pipeline() para manejar toda la cadena.
	 */
	
	while (current)
	{
		if (!current->argv)
		{
			current = current->next;
			continue;
		}
		if (!current->argv[0])
		{
			if (current->redirect_count > 0)
			{
				saved_stdin = dup(STDIN_FILENO);
				status = handle_redirections(current);
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdin);
			}
			current = current->next;
			continue;
		}
		
		// TODO: Aquí detectar si current forma parte de un pipeline
		// if (current->is_piped)  // Necesitarías añadir este campo a t_command
		// {
		//     status = execute_pipeline(current, env);
		//     // Saltar todos los comandos del pipeline
		//     while (current && current->is_piped)
		//         current = current->next;
		//     continue;
		// }
		
		// Comando simple (sin pipe)
		if (is_builtin_command(current))
			status = execute_builtins(current, env);
		else
			status = execute_external_command(current, env);
		current = current->next;
	}
	return (status);
}
