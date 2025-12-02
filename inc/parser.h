#ifndef PARSER_H
# define PARSER_H

// Declaraciones adelantadas (evitar dependencias circulares)
typedef struct s_command t_command;
typedef struct s_env t_env;
typedef struct s_redirect t_redirect;

typedef struct s_quote_state
{
	int	state;
}	t_quote_state;

typedef struct s_pctx
{
	char		**tokens;			// 1. Array de tokens (entrada dividida)
	t_env		*env;				// 2. Environment variables para expansión
	char		**args_temp;		// 3. Argumentos temporales del comando actual
	int			args_count;			// 4. Cuántos argumentos llevamos
	int			args_cap;			// 5. Capacidad del array args_temp
	t_redirect	*redir_temp;		// 6. Redirecciones temporales
	int			redir_count;		// 7. Cuántas redirecciones llevamos
	int			redir_cap;			// 8. Capacidad del array redir_temp
	t_command	*head;				// 9. Primer comando de la lista
	t_command	*curr;				// 10. Comando actual siendo construido
	int			i;					// 11. Índice actual en tokens[]
	int			error;				// 12. Flag de error
	char		*heredoc_delim;		// 13. Delimitador de heredoc (<<)
	int			is_in_pipe;			// 14. Flag: comando actual es parte de un pipe
}	t_pctx;

// ============ Funciones principales del Parser ============
t_command	*parse_input(char *input, t_env *env);
void		free_command(t_command *cmd);
int			validate_syntax(char *input);
void		print_command_list(t_command *head);

// ============ Funciones de contexto (parse_ctx.c) ============
void		reset_ctx(t_pctx *ctx);
void		init_ctx(t_pctx *ctx, char **tokens, t_env *env);
void		free_args_temp(char **args_temp, int args_count);
void		free_redir_temp(t_pctx *ctx);
void		cleanup_resources(t_pctx *ctx);

// ============ Funciones de argumentos (parse_args.c) ============
int			push_arg(t_pctx *ctx, const char *tok);
int			push_redir(t_pctx *ctx, const char *file, int typeval);

// ============ Funciones de nodos (parse_node.c) ============
t_command	*create_node_from_ctx(t_pctx *ctx);
int			finalize_and_append(t_pctx *ctx);

// ============ Funciones handlers (parse_handlers.c) ============
int			handle_pipe(t_pctx *ctx);
int			handle_redir(t_pctx *ctx);

// ============ Funciones de procesamiento (parse_token.c) ============
int			process_token(t_pctx *ctx, char *tok);
void		process_tokens_loop(t_pctx *ctx);

// ============ Funciones de debug (parse_debug.c) ============
void		parser_debug(t_pctx ctx);

// ============ Funciones de expansión (expand_*.c) ============
char		*get_var_name(char *str, int *i);
void		update_quote_state(char c, t_quote_state *qs);
int			should_expand(char c, char next, t_quote_state *qs);
char		*replace_var(char *result, char *var_name, t_env *env);
char		*append_literal(char *result, char *str, int start, int end);

// ============ Funciones de lexer (lexer_*.c) ============
int			ft_isspace(char c);
int			ft_isspecial(char c);
int			ft_get_quoted_size(char *input, int *i);
char		*ft_get_quoted_token(char *input, int *i);
int			ft_skip_quoted_section(char *input, int *i);
char		*ft_get_special_token(char *input, int *i);
char		*ft_get_normal_token(char *input, int *i);
int			ft_count_tokens(char *input);
char		**ft_split_tokens(char *input);
void		ft_free_tokens(char **tokens);

// ============ Funciones de verificación (quote_check.c) ============
int			ft_check_quotes(char *input);
int			ft_check_escape_chars(char *input);

#endif