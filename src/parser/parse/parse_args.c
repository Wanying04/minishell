/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/05 20:14:43 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	realloc_args(t_pctx *ctx)
{
	char	**tmp;
	int		z;
	int		newcap;

	// amplía espacio
	if (ctx->args_cap)
		newcap = ctx->args_cap * 2;
	else
		newcap = 8;
	tmp = ft_calloc(sizeof(char *), newcap);
	if (!tmp)
		return (1);
	z = 0;
	while (z < ctx->args_count)
	{
		tmp[z] = ctx->args_temp[z];
		z++;
	}
	//libera la memoria reservada por malloc a la que apuntaba args_temp
	free(ctx->args_temp);
	ctx->args_temp = tmp;
	ctx->args_cap = newcap;
	return (0);
}
//-poner_argumentos----context del parser, tokens individuales (Ej. "ls", "-la", pero uno a uno)
int	push_arg(t_pctx *ctx, const char *tok)
{
	char	*tilde_expanded;
	char	*expanded;
	//si hace falta más espacio
	if (ctx->args_count + 1 > ctx->args_cap)
	{
		//se lo damos
		if (realloc_args(ctx))
			return (1);
	}
	//primero expandir tilde
	tilde_expanded = expand_tilde((char *)tok, ctx->env);
	//luego expandir variables
	expanded = expand_variables(tilde_expanded, ctx->env, 0);
	if (expanded != tilde_expanded && tilde_expanded != tok)
		free(tilde_expanded);
	ctx->args_temp[ctx->args_count++] = expanded;
	return (0);
}

static int	realloc_redir(t_pctx *ctx)
{
	t_redirect	*tmp;
	int			z;
	int			newcap;

	//Amplía espacio
	if (ctx->redir_cap)
		newcap = ctx->redir_cap * 2;
	else
		newcap = 4;
	tmp = ft_calloc(sizeof(t_redirect), newcap);
	if (!tmp)
		return (1);
	z = 0;
	while (z < ctx->redir_count)
	{
		tmp[z] = ctx->redir_temp[z];
		z++;
	}
	//libera la memoria reservada por malloc a la que apuntaba redir_temp
	free(ctx->redir_temp);
	ctx->redir_temp = tmp;
	ctx->redir_cap = newcap;
	return (0);
}
//-poner_redirecciones--parser context, redirection file, redirection type
int	push_redir(t_pctx *ctx, const char *file, int typeval)
{
	char	*tilde_expanded;
	char	*expanded;
	//si hace falta más espacio
	if (ctx->redir_count + 1 > ctx->redir_cap)
	{
		//se lo damos
		if (realloc_redir(ctx))
			return (1);
	}
	//primero expandir tilde
	tilde_expanded = expand_tilde((char *)file, ctx->env);
	//luego expandir variables
	expanded = expand_variables(tilde_expanded, ctx->env, 0);
	if (expanded != tilde_expanded && tilde_expanded != file)
		free(tilde_expanded);
	//guardamos el redirection file
	ctx->redir_temp[ctx->redir_count].file = expanded;
	//guardamos el redirection type
	ctx->redir_temp[ctx->redir_count].type = typeval;
	//fd sin abrir todavía
	ctx->redir_temp[ctx->redir_count].fd = -1;
	ctx->redir_count++;
	return (0);
}
