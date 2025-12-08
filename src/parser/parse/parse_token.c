/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/08 14:22:06 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Detecta y procesa un token de tipo pipe
static int	process_pipe(t_pctx *ctx, char *tok)
{
	//Si el token es un pipe
	if (ft_strncmp(tok, "|", 2) == 0)
		//Llama a handle_pipe para procesarlo
		//Incrementa el índice y retorna 0 si éxito
		return (handle_pipe(ctx) || (ctx->i++, 0));
	return (-1);//Si no es pipe, seguir probando
}
//Detecta y procesa tokens de redirección
static int	process_redirection(t_pctx *ctx, char *tok)
{
	//Detecta si es ">", ">>", "<"o "<<"
	if (ft_strncmp(tok, ">", 2) == 0 || ft_strncmp(tok, ">>", 3) == 0
		|| ft_strncmp(tok, "<", 2) == 0 || ft_strncmp(tok, "<<", 3) == 0)
	{
		//Si no hay nada después, ERROR
		if (!ctx->tokens[ctx->i + 1])
		{
			ft_putendl_fd("minishell: syntax error: unexpected end of input", 2);
			ctx->error = 2;
			return (2);
		}
		//Procesa la redirección
		if (handle_redir(ctx))
			return (1);
		//Salta dos tokens, operador y archivo
		return (ctx->i += 2, 0);
	}
	return (-1);//Si no es redirección, seguir probando
}
//Clasifica y procesa un token según su tipo
int	process_token(t_pctx *ctx, char *tok)
{
	int	result;
	//Intenta procesar como pipe
	result = process_pipe(ctx, tok);
	if (result != -1)//Si es pipe, entra
		return (result);
	//Intenta procesar como redirección
	result = process_redirection(ctx, tok);
	if (result != -1)//Si es redirección, entra
		return (result);
	//Si no es pipe ni redirección, es un argumento normal
	if (push_arg(ctx, tok))
		return (1);
	return (ctx->i++, 0);
}
//Bucle principal que procesa todos los tokens
void	process_tokens_loop(t_pctx *ctx)
{
	while (ctx->tokens[ctx->i])
	{
		//printf("Token[%i] = %s\n", ctx->i, ctx->tokens[ctx->i]);
		//Procesa el token actual
		if (process_token(ctx, ctx->tokens[ctx->i]))
		{
			//Si hubo error y ctx->error no está marcado, marcar error genérico
			if (ctx->error == 0)
				ctx->error = 1;
			break ;
		}
	}
}
