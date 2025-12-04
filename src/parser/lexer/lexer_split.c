/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/04 15:50:16 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//Básicamente que si hay backslash "\" o semicolon ";" lanza error
static int	check_invalid_chars(char *input)
{
	int	j;

	j = 0;
	while (input[j])
	{
		if (input[j] == '\\')
		{
			ft_putendl_fd("minishell: syntax error: invalid character '\\'",
				2);
			return (1);
		}
		if (input[j] == ';')
		{
			ft_putendl_fd("minishell: syntax error: invalid character ';'", 2);
			return (1);
		}
		j++;
	}
	return (0);
}

/* char	*ft_get_quoted_delimiter(char *input, int *i)
{
	char	*token;
	int		start;

	start = *i;
	token = NULL;
	//Avanza mientras no sea el final, un espacio o un operador especial
	while (input[*i] && !ft_isspace(input[*i]) && !ft_isspecial(input[*i]))
			(*i)++;
	if (*i > start)
		//Si salió del while y quedan cosas por guardar en token, lo hace
		token = ft_join_tokens(token, ft_substr(input, start, *i - start));
	if (token)
		return (token);
	else
		return (ft_strdup(""));
} */

//Llena el array de tokens con lo que haya en el input
static void	fill_tokens(char **tokens, char *input, t_pctx *ctx)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (input[i])
	{
		//Salta espacios
		if (ft_isspace(input[i]))
			i++;
		//Si es un operador especial
		else if (ft_isspecial(input[i]))
			//Llama a una función que lo gestiona
			tokens[k++] = ft_get_special_token(input, &i);
		//Si no es un operador especial: es un comando, argumento o archivo
		else
		{
			if (k > 0 && ft_strncmp(tokens[k - 1], "<<", 3) == 0
			&& (input[i] == '"' || input[i] == '\''))
				ctx->heredoc_dont_expand = 1;
			/* 	tokens[k++] = ft_get_quoted_delimiter(input, &i); */
			//Llama a una función que lo gestiona
			tokens[k++] = ft_get_normal_token(input, &i);
		}
	}
/* 	for(int j = 0; j < k; j++)
		printf("tokens[%i]: %s\n", j, tokens[j]); */
	tokens[k] = NULL;
}
//Divide el input del usuario en tokens individuales. Ej.: "ls", "-la".
char	**ft_split_tokens(char *input, t_pctx *ctx)
{
	char	**tokens;
	int		token_count;

	if (!input)
		return (NULL);
	//Verifica si hay comillas sin cerrar
	if (ft_check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		return (NULL);
	}
	//Verifica si hay caracteres inválidos ("\", ";")
	if (check_invalid_chars(input))
		return (NULL);
	//Cuenta los tokens
	token_count = ft_count_tokens(input);
	//Reserva memoria para el array de tokens. MALLOC.
	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	//Y llena el array con lo que haya en el input
	fill_tokens(tokens, input, ctx);
	return (tokens);
}
