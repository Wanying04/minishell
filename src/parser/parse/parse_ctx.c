/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ctx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/08 14:22:06 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Libera el array temporal de argumentos
void	free_args_temp(char **args_temp, int args_count)
{
	int	z;

	if (!args_temp)
		return ;
	z = 0;
	//Libera cada string individual
	while (z < args_count)
	{
		free(args_temp[z]);
		z++;
	}
	//Libera el array en sí
	free(args_temp);
}

void	free_redir_temp(t_pctx *ctx)
{
	int	z;

	if (!ctx->redir_temp)
		return ;
	z = 0;
	while (z < ctx->redir_count)
	{
		if (ctx->redir_temp[z].file)//Libera out.txt
			free(ctx->redir_temp[z].file);
		z++;
	}
	free(ctx->redir_temp);//Libera el array
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
}

//Resetea campos temporales para procesar un nuevo comando
void	reset_ctx(t_pctx *ctx)
{
	ctx->heredoc_dont_expand = 0;
	//Resetea argumentos temporales y libera si hace falta
	if (ctx->args_temp)
		free_args_temp(ctx->args_temp, ctx->args_count);
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	//Resetea redirecciones temporales y libera si hace falta
	if (ctx->redir_temp)
		free_redir_temp(ctx);
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	//Resetea heredoc y pipe y libera si hace falta
	if (ctx->heredoc_delim)
		free(ctx->heredoc_delim);
	ctx->heredoc_delim = NULL;
	ctx->is_in_pipe = 0;
}
//Inicializa ctx
void	init_ctx(t_pctx *ctx, char **tokens, t_env *env)
{
	//Guarda referencia a tokens y variables de entorno
	if (tokens)
		ctx->tokens = tokens;
	ctx->env = env;
	//Inicializa arrays temporales
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	//Inicializa lista de comandos
	ctx->head = NULL;
	ctx->curr = NULL;
	//Inicializa control
	ctx->i = 0;//Índice actual en tokens
	ctx->error = 0;//Sin errores
	ctx->heredoc_delim = NULL;
	ctx->is_in_pipe = 0;
}

//Limpia todo en caso de error
void	cleanup_resources(t_pctx *ctx)
{
	//Libera args_temp si existe
	if (ctx->args_temp)
	{
		free_args_temp(ctx->args_temp, ctx->args_count);
		ctx->args_temp = NULL;
		ctx->args_count = 0;
		ctx->args_cap = 0;
	}
	//Libera redir_temp si existe
	if (ctx->redir_temp)
		free_redir_temp(ctx);
	//Libera heredoc_delim si existe
	if (ctx->heredoc_delim)
	{
		free(ctx->heredoc_delim);
		ctx->heredoc_delim = NULL;
	}
	//Libera toda la lista de comandos si existe
	if (ctx->head)
	{
		free_command(ctx->head);
		ctx->head = NULL;
		ctx->curr = NULL;
	}
}
