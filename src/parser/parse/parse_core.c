/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/08 18:11:14 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	finalize_parsing(t_pctx *ctx)
{
	//Si no hubo error y hay argumentos o redirecciones pendientes
	if (!ctx->error && (ctx->args_count > 0 || ctx->redir_count > 0))
	{
		//Crea el último nodo y lo añade a la lista
		if (finalize_and_append(ctx))
			ctx->error = 1;
	}
	//Si hay redirecciones sin comando, las libera
	else if (ctx->redir_temp)
		free_redir_temp(ctx);
	//Libera el delimitador de heredoc si existe
	if (ctx->heredoc_delim)
	{
		free(ctx->heredoc_delim);
		ctx->heredoc_delim = NULL;
	}
	return (ctx->error);
}
//La función principal de todo el parser, recibe el input y las variables de entorno
t_command	*parse_input(char *input, t_env *env)
{
	char	**tokens;
	t_pctx	ctx;

	//Inicializa el campo necesario para ft_split_tokens
	ctx.heredoc_dont_expand = 0;
	//Extrae los tokens del input y los guarda en un array de strings. MALLOC.
	tokens = ft_split_tokens(input, &ctx); //["echo", "hello", ">", "out.txt", "|", "cat", "out.txt"]
	if (!tokens)
	{
		env->exit_status = SYNTAX_ERROR;
		return (NULL);
	}
	//Inicializa el contexto, una estructura (t_pctx ctx)
	init_ctx(&ctx, tokens, env);
	//que servirá para organizar los tokens y etiquetarlos
	//para entregárselos al executor ya procesados con process_token_loop
	process_tokens_loop(&ctx);
	//Modo para debuguear
	if (getenv("PARSER_DEBUG2"))
	{
		parser_debug(ctx);
		ft_free_tokens(tokens);
		if (ctx.error)
			return (cleanup_resources(&ctx), NULL);
		if (ctx.head)
			free_command(ctx.head);
		return (NULL);
	}
	//Finaliza el último comando y libera recursos temporales si existen
	if (ctx.args_count == 1 && ctx.args_temp[0][0] == '\0' && 
		(ft_strncmp(input, "\"\"", 3) == 0 || ft_strncmp(input, "''", 3) == 0))
	{
		write(2, "Command '' not found\n", 21);
		cleanup_resources(&ctx);
		env->exit_status = 127;
		return (NULL);
	}
	finalize_parsing(&ctx);
	//Libera el array de tokens originales (ya copiados a node->argv)
	ft_free_tokens(tokens);
	//Si hubo error, limpia todo y retorna NULL
	if (ctx.error)
	{
		env->exit_status = SYNTAX_ERROR;
		return (cleanup_resources(&ctx), NULL);
	}
	//Retorna la lista de comandos
	return (ctx.head);
}
