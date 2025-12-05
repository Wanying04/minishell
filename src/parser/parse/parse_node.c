/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/05 20:15:34 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Transfiere los punteros de ctx->args_temp a node->argv
static void	copy_argv_to_node(t_command *node, t_pctx *ctx)
{
	int	j;

	j = 0;
	//recorremos ctx->args_temp,
	//copiando el puntero que apunta a cada token del comando,
	//a node->argv
	while (j < ctx->args_count)
	{
		node->argv[j] = ctx->args_temp[j];
		j++;
	}
	node->argv[ctx->args_count] = NULL;
	free(ctx->args_temp);
}
//Transfiere las redirecciones del contexto al nodo
static void	set_redirects(t_command *node, t_pctx *ctx)
{
	//si hay redirecciones las transferismos
	if (ctx->redir_count > 0)
	{
		node->redirects = ctx->redir_temp;
		node->redirect_count = ctx->redir_count;
		//reseteamos ctx
		ctx->redir_temp = NULL;
		ctx->redir_count = 0;
		ctx->redir_cap = 0;
	}
	//si no, inicializamos a NULL y cero.
	else
	{
		node->redirects = NULL;
		node->redirect_count = 0;
	}
}
/* ¿Por qué argv hace nuevo malloc pero redirects no?

Respuesta: Optimización de memoria.

args_temp tiene capacidad 8, 16, 32... (puede desperdiciar mucho)
redir_temp tiene capacidad 4, 8... (desperdicia poco, no vale la pena copiar) */
t_command	*create_node_from_ctx(t_pctx *ctx)
{
	t_command	*node;

	//malloc estructura t_command
	node = ft_calloc(sizeof(t_command), 1);
	if (!node)
		return (NULL);
	//malloc array de punteros (tamaño exacto).
	//ej.: para ["echo", "hello", "world"] se hace malloc(sizeof(char *) x (3+1))
	node->argv = ft_calloc(sizeof(char *), (ctx->args_count + 1));
	if (!node->argv)
		return (free(node), NULL);
	//copia los punteros a "echo", "hello" y "world" de ctx->args_temp a node->argv
	copy_argv_to_node(node, ctx);
	//transfiere el puntero ctx->redir_temp y ctx->redir_count
	set_redirects(node, ctx);
	//transfiere heredoc_delimiter
	node->heredoc_delimiter = ctx->heredoc_delim;
	node->dont_expand = ctx->heredoc_dont_expand;
	ctx->heredoc_delim = NULL;
	//aprovechamos para inicializar otros campos de la estructura t_command
	node->is_builtin = 0;
	node->is_piped = ctx->is_in_pipe;
	node->next = NULL;
	node->prev = NULL;
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	return (node);
}
//Finalizar comando y añadirlo a la lista (estructura parser_context)
int	finalize_and_append(t_pctx *ctx)
{
	t_command	*node;

	//Aquí se hace malloc de t_command y del array argv, se pasa args_temp a node->argv,
	//redir_temp a node->redirects y se resetea ctx para el siguiente comando
	node = create_node_from_ctx(ctx);
	//Si falla malloc
	if (!node)
		return (1);
	//Si es el primer comando, ctx->head apunta a él
	if (!ctx->head)
		ctx->head = node;
	if (ctx->curr/*si existe ya un comando*/)
	{
		ctx->curr->next = node;//que curr->next apunte al nuevo (node)
		node->prev = ctx->curr;//que el previo del nuevo (node->prev) apunte a ctx->curr
	}
	ctx->curr = node;//node (el nuevo) se convierte en ctx->curr
	return (0);
}
