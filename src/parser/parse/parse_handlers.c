/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/04 15:44:12 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Valida la sintaxis del pipe
static int	check_pipe_syntax(t_pctx *ctx)
{
	char	*next_tok;
	//Obtiene el siguiente token después del pipe
	next_tok = ctx->tokens[ctx->i + 1];
	//Si no hay nada después del pipe, ERROR 
	if (!next_tok)
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of input", 2);
		ctx->error = 2;
		return (2);
	}
	//Si lo que hay después es otro pipe, ERROR
	if (ft_strncmp(next_tok, "|", 2) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 2;
		return (2);
	}
	return (0);
}
//Procesa un token de redirección
int	handle_pipe(t_pctx *ctx)
{
	//Si no hay argumentos antes, ERROR
	if (ctx->args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 2;
		return (2);
	}
	//Valida la sintaxis del pipe, si no hay argumentos después o es otro pipe, ERROR
	if (check_pipe_syntax(ctx))
		return (2);
	//Si todo va bien, marca que estamos en un pipe y finaliza el comando actual
	if (finalize_and_append(ctx))
		return (1);
	//Resetea el contexto para el próximo comando del pipe
	reset_ctx(ctx);
	ctx->is_in_pipe = 1;
	return (0);
}
//Determina el tipo de redirección
static int	get_redir_type(char *tok)
{
	if (ft_strncmp(tok, ">>", 3) == 0)
		return (REDIR_APPEND);
	else if (ft_strncmp(tok, ">", 2) == 0)
		return (REDIR_OUT);
	else if (ft_strncmp(tok, "<<", 3) == 0)
		return (REDIR_HEREDOC);
	else if (ft_strncmp(tok, "<", 2) == 0)
		return (REDIR_IN);
	return (0);
}

int	handle_redir(t_pctx *ctx)
{
	char	*tok;
	int		type;
	int		idx;

	//Obtiene el token de redirección actual
	tok = ctx->tokens[ctx->i];
	//printf("ctx->tokens[%i]: %s\n", ctx->i, ctx->tokens[idx]);
	//Identifica qué tipo de redirección es
	type = get_redir_type(tok);
	//El siguiente token debe ser el archivo/delimitador
	idx = ctx->i + 1;
	//Si no hay nada después o el siguiente token es otro operador especial, ERROR
	if (!ctx->tokens[idx] || ft_isspecial(ctx->tokens[idx][0]))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		ctx->error = 2;
		return (2);
	}
	//Si es heredoc, guarda el delimitador
	if (type == REDIR_HEREDOC)
		ctx->heredoc_delim = ft_strdup(ctx->tokens[idx]);
	//Añade la redirección a la lista del comando (file, type y fd)
	if (push_redir(ctx, ctx->tokens[idx], type))
		return (1);
	return (0);
}
