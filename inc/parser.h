/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 00:00:00 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:35:10 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command	t_command;
typedef struct s_env		t_env;
typedef struct s_redirect	t_redirect;

typedef struct s_quote_state
{
	int	state;
}	t_quote_state;

typedef struct s_pctx
{
	int			heredoc_dont_expand;
	char		**tokens;
	t_env		*env;
	char		**args_temp;
	int			args_count;
	int			args_cap;
	t_redirect	*redir_temp;
	int			redir_count;
	int			redir_cap;
	t_command	*head;
	t_command	*curr;
	int			i;
	int			error;
	char		*heredoc_delim;
	int			is_in_pipe;
}	t_pctx;

t_command	*parse_input(char *input, t_env *env);
void		free_command(t_command *cmd);
int			validate_syntax(char *input);
void		reset_ctx(t_pctx *ctx);
void		init_ctx(t_pctx *ctx, char **tokens, t_env *env);
void		free_args_temp(char **args_temp, int args_count);
void		free_redir_temp(t_pctx *ctx);
void		cleanup_resources(t_pctx *ctx);
int			push_arg(t_pctx *ctx, const char *tok);
int			push_redir(t_pctx *ctx, const char *file, int typeval);
t_command	*create_node_from_ctx(t_pctx *ctx);
int			finalize_and_append(t_pctx *ctx);
int			handle_pipe(t_pctx *ctx);
int			handle_redir(t_pctx *ctx);
int			process_token(t_pctx *ctx, char *tok);
void		process_tokens_loop(t_pctx *ctx);
char		*get_var_name(char *str, int *i);
void		update_quote_state(char c, t_quote_state *qs);
int			should_expand(char c, char next, t_quote_state *qs);
char		*replace_var(char *result, char *var_name, t_env *env);
char		*append_literal(char *result, char *str, int start, int end);
char		*append_char(char *str, char c);
int			ft_isspace(char c);
int			ft_isspecial(char c);
char		*ft_get_quoted_token(char *input, int *i);
int			ft_skip_quoted_section(char *input, int *i);
char		*ft_get_quoted_delimiter(char *input, int *i);
char		*ft_get_special_token(char *input, int *i);
char		*ft_get_normal_token(char *input, int *i);
char		*ft_join_tokens(char *s1, char *s2);
int			ft_count_tokens(char *input);
char		**ft_split_tokens(char *input, t_pctx *ctx);
void		ft_free_tokens(char **tokens);
int			ft_check_quotes(char *input);
char		*handle_empty_quotes(char *token, char quote, char *input, int *i);
char		*extract_quoted_content(char *token, char *input, int *i);

#endif