#ifndef PARSER_H
# define PARSER_H

// Declaraciones adelantadas (evitar dependencias circulares)
typedef struct s_command t_command;
typedef struct s_env t_env;
typedef struct s_redirect t_redirect;  // ← AÑADIR ESTA LÍNEA

typedef struct s_pctx
{
	char		**tokens;			// 1. Array de tokens (entrada dividida)
	char		**args_temp;		// 2. Argumentos temporales del comando actual
	int			args_count;			// 3. Cuántos argumentos llevamos
	int			args_cap;			// 4. Capacidad del array args_temp
	t_redirect	*redir_temp;		// 5. Redirecciones temporales
	int			redir_count;		// 6. Cuántas redirecciones llevamos
	int			redir_cap;			// 7. Capacidad del array redir_temp
	char		*heredoc_delim;		// 8. Delimitador de heredoc (<<)
	t_command	*head;				// 9. Primer comando de la lista
	t_command	*curr;				// 10. Comando actual siendo construido
	int			i;					// 11. Índice actual en tokens[]
	int			error;				// 12. Flag de error
	t_env		*env;				// 13. Environment variables para expansión
	int			is_in_pipe;			// 14. Flag: comando actual es parte de un pipe
}	t_pctx;

// ============ Funciones principales del Parser ============
t_command	*parse_input(char *input, t_env *env);
void		free_command(t_command *cmd);
int			validate_syntax(char *input);
void		print_command_list(t_command *head);

// ============ Funciones auxiliares ============
int		ft_isspace(char c);
int		ft_isspecial(char c);
int		ft_get_quoted_size(char *input, int *i);
char	*ft_get_quoted_token(char *input, int *i);
int		ft_skip_quoted_section(char *input, int *i);
char	*ft_get_special_token(char *input, int *i);
char	*ft_get_normal_token(char *input, int *i);
int		ft_count_tokens(char *input);
char	**ft_split_tokens(char *input);
int		ft_check_quotes(char *input);
int		ft_check_escape_chars(char *input);
void	ft_free_tokens(char **tokens);

#endif